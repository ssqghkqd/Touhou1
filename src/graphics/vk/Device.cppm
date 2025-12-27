// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
//
module;
#include <vulkan/vulkan.h>

#include <format>
#include <set>
#include <vector>
export module vk:Device;
import defs;
import spdlog;

import :common;

namespace th::vk
{
bool checkDeviceExtensionsSupport(VkPhysicalDevice physicalDevice,
                                  const std::vector<const char*>& extensions)
{
    uint32_t extensionCount{0};
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());
    std::vector<const char*> availableStr{};
    availableStr.reserve(extensionCount);
    for (const auto& extension : availableExtensions)
    {
        availableStr.push_back(extension.extensionName);
    }
    return isSubset(availableStr, extensions);
}

struct DeviceInfo
{
    std::string deviceName_{};
    std::string deviceType_{};
    VkDeviceSize dedicatedVRAM_{0};
    VkDeviceSize sharedRAM_{0};
    std::string apiVersion_{};
    uint32_t driverVersion_{};

    void printDeviceInfo()
    {
        spdlog::info("设备信息:\n设备名称:{}\n设备类型:{}\n独占显存大小:{}MB\n共享显存大小:{}MB\nVulkan版本:{}\n驱动版本:{}",
                     deviceName_,
                     deviceType_,
                     (int)dedicatedVRAM_,
                     (int)sharedRAM_,
                     apiVersion_,
                     (int)driverVersion_);
    }
};

DeviceInfo parseDeviceInfo(VkPhysicalDevice physicalDevice)
{
    DeviceInfo deviceInfo{};
    VkPhysicalDeviceProperties props{};
    vkGetPhysicalDeviceProperties(physicalDevice, &props);
    VkPhysicalDeviceMemoryProperties memProps{};
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProps);

    deviceInfo.deviceName_ = props.deviceName;
    switch (props.deviceType)
    {
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            deviceInfo.deviceType_ = "集成显卡";
            break;
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            deviceInfo.deviceType_ = "独立显卡";
            break;
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            deviceInfo.deviceType_ = "虚拟显卡";
            break;
        default:
            deviceInfo.deviceType_ = "未知类型显卡";
            break;
    }

    VkDeviceSize dedicatedMemory{0};
    VkDeviceSize sharedMemory{0};
    for (uint32_t i = 0; i < memProps.memoryHeapCount; i++)
    {
        bool heapHasHostVisible = false;

        // 检查这个堆中是否有主机可见的内存类型
        for (uint32_t j = 0; j < memProps.memoryTypeCount; j++)
        {
            if (memProps.memoryTypes[j].heapIndex == i)
            {
                if (memProps.memoryTypes[j].propertyFlags & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT)
                {
                    heapHasHostVisible = true;
                    break;
                }
            }
        }

        VkDeviceSize heapSize = memProps.memoryHeaps[i].size;

        if (heapHasHostVisible)
        {
            // 有主机可见类型 -> 这是共享内存
            sharedMemory += heapSize;
        }
        else
        {
            // 没有主机可见类型 -> 可能是专用显存
            // 但还要检查是否真的是DEVICE_LOCAL
            if (memProps.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
            {
                dedicatedMemory += heapSize;
            }
            else
            {
                sharedMemory += heapSize; // 非DEVICE_LOCAL的也是共享
            }
        }
    }

    deviceInfo.dedicatedVRAM_ = dedicatedMemory / (1000 * 1000);
    deviceInfo.sharedRAM_ = sharedMemory / (1000 * 1000);

    deviceInfo.apiVersion_ = std::format("{}.{}.{}",
                                         VK_API_VERSION_MAJOR(props.apiVersion),
                                         VK_API_VERSION_MINOR(props.apiVersion),
                                         VK_API_VERSION_PATCH(props.apiVersion));
    deviceInfo.driverVersion_ = props.driverVersion;

    return deviceInfo;
}

export class Device
{
  private:
    VkPhysicalDevice physicalDevice_{nullptr};
    VkDevice device_{nullptr};
    VkQueue graphicsQueue_{nullptr};
    VkQueue presentQueue_{nullptr};
    std::vector<const char*> deviceExtensions_{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VkSurfaceKHR surface_{nullptr};
    VkInstance instance_{nullptr};

  public:
    explicit Device(VkSurfaceKHR surface, VkInstance instance)
        : surface_(surface), instance_(instance)
    {
    }
    ~Device()
    {
        if (device_ != nullptr)
        {
            vkDestroyDevice(device_, nullptr);
            spdlog::debug("vk：销毁设备");
        }
    }

    void waitIdle() const
    {
        vkDeviceWaitIdle(device_);
    }

  private:
    friend class Renderer;

    operr createLogicalDevice()
    {
        const QueueFamilyIndices indices = findQueueFamilies(physicalDevice_, surface_);
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()}; // set的意义是有可能两个队列是同一个
        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures{};

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = (uint32_t)deviceExtensions_.size();
        createInfo.ppEnabledExtensionNames = deviceExtensions_.data();

        VkResult result = vkCreateDevice(physicalDevice_, &createInfo, nullptr, &device_);
        if (result != VK_SUCCESS)
        {
            spdlog::critical("vk:创建逻辑设备失败");
            return err::vk_device_failed;
        }

        vkGetDeviceQueue(device_, indices.graphicsFamily.value(), 0, &graphicsQueue_);
        vkGetDeviceQueue(device_, indices.presentFamily.value(), 0, &presentQueue_);

        spdlog::debug("vk:创建逻辑设备");
        return {};
    }
    operr pickPhysicalDevice()
    {
        uint32_t deviceCount{0};
        vkEnumeratePhysicalDevices(instance_, &deviceCount, nullptr);
        if (deviceCount == 0)
        {
            spdlog::critical("vk:没有物理设备");
            return err::vk_physical_device_failed;
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance_, &deviceCount, devices.data());
        spdlog::info("找到如下设备:");
        for (const auto& device : devices)
        {
            auto detail = parseDeviceInfo(device);
            detail.printDeviceInfo();
        }
        for (const auto& device : devices)
        {
            if (isDeviceSuitable(device))
            {
                physicalDevice_ = device;
                spdlog::info("选择设备:");
                auto detail = parseDeviceInfo(device);
                detail.printDeviceInfo();
                break;
            }
        }

        if (physicalDevice_ == VK_NULL_HANDLE)
        {
            spdlog::critical("没有可用的物理设备");
            return err::vk_physical_device_failed;
        }
        return {};
    }

    bool isDeviceSuitable(VkPhysicalDevice physicalDevice) const
    {
        const QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface_);
        const bool extensionsSupported = checkDeviceExtensionsSupport(physicalDevice, deviceExtensions_);

        bool swapChainAdequate = false;
        if (extensionsSupported)
        {
            const SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface_);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        return indices.isComplete() && extensionsSupported && swapChainAdequate;
    }
};
} // namespace th::vk
