// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
// vk上下文主文件
module;
#include <vulkan/vulkan.h>

#include <optional>
#include <set>
#include <vector>
export module vk:vkcore;
import spdlog;
import glfw;

import defs;
import :vkdefs;

#ifdef NDEBUG
constexpr bool enableValidation = false;
#else
constexpr bool enableValidation = true;
#endif

bool isSubset(const std::vector<const char*>& superset, // 父集
              const std::vector<const char*>& subset)   // 子集
{
    for (const char* sub : subset)
    { // 子集的每个元素
        bool found = false;
        for (const char* super : superset)
        { // 在父集里找
            if (strcmp(super, sub) == 0)
            {
                found = true;
                break;
            }
        }
        if (!found)
        {
            return false;
        } // 子集元素不在父集中
    }
    return true; // 子集所有元素都在父集中
}

bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers)
{
    uint32_t layerCount{0};
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    std::vector<const char*> availableStr{};
    availableStr.reserve(availableLayers.size());
    for (const auto& layer : availableLayers)
    {
        availableStr.push_back(layer.layerName);
    }

    return isSubset(availableStr, validationLayers);
}

bool checkInstanceExtensionsSupport(const std::vector<const char*>& extensions)
{
    uint32_t extensionCount{0};
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
    std::vector<const char*> availableStr{};
    availableStr.reserve(extensionCount);
    for (const auto& extension : availableExtensions)
    {
        availableStr.push_back(extension.extensionName);
    }
    return isSubset(availableStr, extensions);
}
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

th::QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    th::QueueFamilyIndices indices{};
    uint32_t queueFamilyCount{0};
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    for (uint32_t i = 0; i < queueFamilyCount; i++)
    {
        // 检查是否支持图形命令
        if (queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
        }

        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
        if (presentSupport)
        {
            indices.presentFamily = i;
        }

        if (indices.isComplete())
        {
            break;
        }
    }

    return indices;
}
th::SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
{
    th::SwapChainSupportDetails details;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

    if (formatCount != 0)
    {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
    }

    // 3. 呈现模式
    uint32_t presentModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
    if (presentModeCount != 0)
    {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device,
                                                  surface,
                                                  &presentModeCount,
                                                  details.presentModes.data());
    }

    return details;
}

VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    [[maybe_unused]] VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    [[maybe_unused]] void* pUserData)
{
    if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    {
        spdlog::error("Vulkan Error: {}", pCallbackData->pMessage);
        throw std::runtime_error("Vulkan Error");
    }
    else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
        spdlog::warn("Vulkan Warning: {}", pCallbackData->pMessage);
    }
    else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
    {
        spdlog::info("Vulkan Info: {}", pCallbackData->pMessage);
    }
    else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
    {
        spdlog::debug("Vulkan Debug: {}", pCallbackData->pMessage);
    }

    return VK_FALSE;
}

namespace th
{
export class vkcore
{
  private:
    VkInstance instance_{nullptr};
    VkDebugUtilsMessengerEXT debugMessenger_{nullptr};
    VkPhysicalDevice physicalDevice_{nullptr};
    VkDevice device_{nullptr};

    std::vector<const char*> validationLayers_{"VK_LAYER_KHRONOS_validation"};
    std::vector<const char*> deviceExtensions_{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

  public:
    vkcore() = default;
    ~vkcore() = default;
    operr init(const char* appName, const char* engineName, uint32_t extensionCount, const char** extensions)
    {
        spdlog::debug("开始初始化vulkan, 应用名{}, 引擎:{}", appName, engineName);
        auto e = createInstance(appName, engineName, extensionCount, extensions);
        if (e.has_value())
            return e;
        initDebugMessenger();

        return {};
    }

    operr initDevice(VkSurfaceKHR surface)
    {
        auto e = pickPhysicalDevice(surface);
        if (e.has_value())
            return e;
        e = createLogicalDevice(surface);
        if (e.has_value())
            return e;

        return {};
    }

    [[nodiscard]] VkDevice getDevice() const noexcept
    {
        return device_;
    }
    [[nodiscard]] VkPhysicalDevice getPhysicalDevice() const noexcept
    {
        return physicalDevice_;
    }
    [[nodiscard]] VkInstance getInstance() const noexcept
    {
        return instance_;
    }

    vkcore(const vkcore&) = delete;
    vkcore(vkcore&& other) = default;
    vkcore& operator=(vkcore&& other) = default;
    vkcore& operator=(const vkcore&) = delete;

  private:
    operr createInstance(const char* appName, const char* engineName, uint32_t extensionCount, const char** extensions)
    {
        // 1.验证验证层
        if (enableValidation && !checkValidationLayerSupport(validationLayers_))
        {
            spdlog::critical("验证层不支持");
            return err::vk_validation_not_supported;
        }

        // 2.创建实例信息
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = appName;
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = engineName;
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_4;

        // 3.验证扩展
        std::vector extensionsList(extensions, extensions + extensionCount);
        if (enableValidation)
        {
            extensionsList.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
        if (!checkInstanceExtensionsSupport(extensionsList))
        {
            spdlog::critical("有扩展不支持");
            return err::vk_instance_extensions_failed;
        }

        // 4.创建实例
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensionsList.size());
        createInfo.ppEnabledExtensionNames = extensionsList.data();
        if (enableValidation)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers_.size());
            createInfo.ppEnabledLayerNames = validationLayers_.data();
        }
        else
        {
            // ReSharper disable once CppDFAUnreachableCode
            createInfo.enabledLayerCount = 0;
        }
        if (vkCreateInstance(&createInfo, nullptr, &instance_) != VK_SUCCESS)
        {
            return err::vk_instance_failed;
        }
        spdlog::debug("vk:创建实例");
        return {};
    }

    void initDebugMessenger()
    {
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debugCreateInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                          VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                          VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debugCreateInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debugCreateInfo.pfnUserCallback = debugCallback;
        debugCreateInfo.pUserData = nullptr; // Optional

        const auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance_, "vkCreateDebugUtilsMessengerEXT");
        func(instance_, &debugCreateInfo, nullptr, &debugMessenger_);
        spdlog::debug("vk:初始化验证层信使");
    }

    operr pickPhysicalDevice(VkSurfaceKHR surface)
    {
        uint32_t deviceCount{0};
        vkEnumeratePhysicalDevices(instance_, &deviceCount, nullptr);
        if (deviceCount == 0)
        {
            spdlog::critical("没有物理设备");
            return err::vk_physical_device_failed;
        }

        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance_, &deviceCount, devices.data());
        for (const auto& device : devices)
        {
#ifndef NDEBUG
            VkPhysicalDeviceProperties props1{};
            vkGetPhysicalDeviceProperties(device, &props1);
            spdlog::debug("vk:尝试设备:{}", props1.deviceName);
#endif
            if (isDeviceSuitable(device, surface))
            {
                physicalDevice_ = device;
                VkPhysicalDeviceProperties props{};
                vkGetPhysicalDeviceProperties(physicalDevice_, &props);
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
    operr createLogicalDevice(VkSurfaceKHR surface)
    {
        const QueueFamilyIndices indices = findQueueFamilies(physicalDevice_, surface);
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
            spdlog::critical("创建逻辑设备失败");
            return err::vk_device_failed;
        }
        spdlog::debug("vk:创建逻辑设备");
        return {};
    }

    bool isDeviceSuitable(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface) const
    {
        const QueueFamilyIndices indices = findQueueFamilies(physicalDevice, surface);
        const bool extensionsSupported = checkDeviceExtensionsSupport(physicalDevice, deviceExtensions_);

        bool swapChainAdequate = false;
        if (extensionsSupported)
        {
            const SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice, surface);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        return indices.isComplete() && extensionsSupported && swapChainAdequate;
    }
};
} // namespace th