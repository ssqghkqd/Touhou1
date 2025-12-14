// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
// vk上下文主文件
module;
#include <vulkan/vulkan.h>

#include <cstdint>
#include <optional>
#include <set>
#include <vector>
export module vk:VKContext;
import spdlog;
import glfw;
import :utils;
import defs;

#ifdef NDEBUG
constexpr bool enableValidation = false;
#else
constexpr bool enableValidation = true;
#endif

namespace th::vk
{
struct QueueFamilyIndices
{
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;
    bool isComplete() const
    {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

struct SwapChainSupportDetails
{
    VkSurfaceCapabilitiesKHR capabilities{};
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;
};

export class VKContext
{
  private:
    VkInstance instance_{nullptr};
    VkDebugUtilsMessengerEXT debugMessenger_{nullptr};
    const std::vector<const char*> validationLayers_{"VK_LAYER_KHRONOS_validation"};

    VkSurfaceKHR surface_{nullptr};
    VkPhysicalDevice physicalDevice_{nullptr};
    VkDevice device_{nullptr};
    VkQueue graphicsQueue_{nullptr};
    VkQueue presentQueue_{nullptr};
    const std::vector<const char*> deviceExtensions_{VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VkSwapchainKHR swapChain_{nullptr};
    std::vector<VkImage> swapChainImages_{};
    VkFormat swapChainImageFormat_{};
    VkExtent2D swapChainExtent_{};

  public:
    operr init(uint32_t extensionCount,
               const char** extensions,
               glfw::window* window)
    {
        auto e = createInstance("game", "no engine", extensionCount, extensions);
        if (e.has_value())
            return e;

        setupDebugMessenger();

        e = createSurface(window);
        if (e.has_value())
            return e;
        e = pickPhysicalDevice();
        if (e.has_value())
            return e;
        e = createLogicalDevice();
        if (e.has_value())
            return e;
        e = createSwapChain(window);
        if (e.has_value())
            return e;

        return std::nullopt;
    }

    ~VKContext()
    {
        cleanup();
    }

    void cleanup() const
    {
        vkDestroySwapchainKHR(device_, swapChain_, nullptr);
        vkDestroyDevice(device_, nullptr);
        vkDestroySurfaceKHR(instance_, surface_, nullptr);
        if (enableValidation)
        {
            const auto destroyFunc = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance_, "vkDestroyDebugUtilsMessengerEXT");
            destroyFunc(instance_, debugMessenger_, nullptr);
        }
        vkDestroyInstance(instance_, nullptr);
    }

  private:
    //================================实例============================
    operr createInstance(const char* appName,
                         const char* engineName,
                         uint32_t extensionCount,
                         const char** extensions)
    {
        // 1.验证验证层
        if (enableValidation && !checkValidationLayerSupport(validationLayers_))
        {
            spdlog::critical("vk:验证层不支持");
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
            spdlog::critical("vk:有实例扩展不支持");
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
            createInfo.enabledLayerCount = 0;
        }
        const VkResult result = vkCreateInstance(&createInfo, nullptr, &instance_);
        if (result != VK_SUCCESS)
        {
            spdlog::critical("vk:创建VK实例失败");
            return err::vk_instance_extensions_failed;
        }
        spdlog::debug("vk:创建实例成功");
        return std::nullopt;
    }
    // ================================验证层===============================
    void setupDebugMessenger()
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
    }

    // ==================================表面================================
    operr createSurface(glfw::window* window)
    {
        const VkResult result = glfw::createWindowSurface(instance_, window, nullptr, &surface_);
        if (result != VK_SUCCESS)
        {
            spdlog::critical("创建表面失败");
            return err::vk_surface_failed;
        }
        return std::nullopt;
    }
    // ====================================选取设备==============================
    operr pickPhysicalDevice()
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
            if (isDeviceSuitable(device))
            {
                physicalDevice_ = device;
                VkPhysicalDeviceProperties props{};
                vkGetPhysicalDeviceProperties(physicalDevice_, &props);
                spdlog::info("选择设备: {}, 类型: {}",
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
        return std::nullopt;
    }

    operr createLogicalDevice()
    {
        const QueueFamilyIndices indices = findQueueFamilies(physicalDevice_);
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

        vkGetDeviceQueue(device_, indices.graphicsFamily.value(), 0, &graphicsQueue_);
        vkGetDeviceQueue(device_, indices.presentFamily.value(), 0, &presentQueue_);
        return std::nullopt;
    }

    // ========================================交换连===============================
    operr createSwapChain(glfw::window* window)
    {
        const SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice_);

        const VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        const VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        const VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities, window);

        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
        {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = surface_;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        QueueFamilyIndices indices = findQueueFamilies(physicalDevice_);
        const uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()};

        if (indices.graphicsFamily != indices.presentFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;     // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        /*字段指定是否应使用 alpha 通道与窗口系统中的其他窗口进行混合。你几乎总是希望简单地忽略 alpha 通道*/
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;
        /*使用 Vulkan，当你的应用程序运行时，你的交换链可能会失效或未优化，例如因为窗口大小已调整。在这种情况下，实际上需要从头开始重新创建交换链，并且必须在此字段中指定对旧交换链的引用。这是一个复杂的主题，我们将在未来的章节中了解更多。现在，我们假设我们只会创建一个交换链。*/

        const VkResult result = vkCreateSwapchainKHR(device_, &createInfo, nullptr, &swapChain_);
        if (result != VK_SUCCESS)
        {
            spdlog::critical("交换链创建失败");
            return err::vk_swapchain_failed;
        }

        vkGetSwapchainImagesKHR(device_, swapChain_, &imageCount, nullptr);
        swapChainImages_.resize(imageCount);
        vkGetSwapchainImagesKHR(device_, swapChain_, &imageCount, swapChainImages_.data());

        swapChainExtent_ = extent;
        swapChainImageFormat_ = surfaceFormat.format;
        return std::nullopt;
    }

    // ==================================工具函数===============================
    bool isDeviceSuitable(VkPhysicalDevice physicalDevice) const
    {
        const QueueFamilyIndices indices = findQueueFamilies(physicalDevice);
        const bool extensionsSupported = checkDeviceExtensionsSupport(physicalDevice, deviceExtensions_);

        bool swapChainAdequate = false;
        if (extensionsSupported)
        {
            const SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
        }

        return indices.isComplete() && extensionsSupported && swapChainAdequate;
    }

    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) const
    {
        QueueFamilyIndices indices{};
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
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface_, &presentSupport);
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

    SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device) const
    {
        SwapChainSupportDetails details;
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface_, &details.capabilities);

        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &formatCount, nullptr);

        if (formatCount != 0)
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface_, &formatCount, details.formats.data());
        }

        // 3. 呈现模式
        uint32_t presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface_, &presentModeCount, nullptr);
        if (presentModeCount != 0)
        {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device,
                                                      surface_,
                                                      &presentModeCount,
                                                      details.presentModes.data());
        }

        return details;
    }
};
} // namespace th::vk