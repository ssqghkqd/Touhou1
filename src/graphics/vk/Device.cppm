// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
//
module;
#include <vulkan/vulkan.h>

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
        for (const auto& device : devices)
        {
            VkPhysicalDeviceProperties props{};
            vkGetPhysicalDeviceProperties(device, &props);
            spdlog::debug("vk:尝试设备:{}", props.deviceName);
            if (isDeviceSuitable(device))
            {
                physicalDevice_ = device;
                spdlog::info("==============选择设备: {}, 类型: {}============",
                             props.deviceName,
                             static_cast<int>(props.deviceType));
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
