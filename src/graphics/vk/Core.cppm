// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
//
module;
#include <vulkan/vulkan.h>

#include <vector>
export module vk:Core;
import defs;
import spdlog;

import :common;

#ifdef NDEBUG
constexpr bool enableValidation = false;
#else
constexpr bool enableValidation = true;
#endif

namespace th::vk
{
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

export class Core
{
  private:
    VkInstance instance_{nullptr};
    VkDebugUtilsMessengerEXT debugMessenger_{nullptr};

    std::vector<const char*> validationLayers_{"VK_LAYER_KHRONOS_validation"};

  public:
    Core() = default;
    ~Core()
    {
        if (enableValidation && debugMessenger_ != nullptr)
        {
            const auto destroyFunc = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance_, "vkDestroyDebugUtilsMessengerEXT");
            destroyFunc(instance_, debugMessenger_, nullptr);
            spdlog::debug("vk:销毁调试信使");
        }
        if (instance_ != nullptr)
        {
            vkDestroyInstance(instance_, nullptr);
            spdlog::debug("vk:销毁实例");
        }
    }

  private:
    friend class Renderer;
    operr createInstance(const char* appName, const char* engineName, uint32_t extensionCount, const char** extensions)
    {
        // 1.验证验证层
        if (enableValidation && !checkValidationLayerSupport(validationLayers_))
        {
            spdlog::critical("vk实例:验证层不支持");
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
            spdlog::critical("vk实例:有扩展不支持");
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
};
} // namespace th::vk
