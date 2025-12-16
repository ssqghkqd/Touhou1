// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
// vk上下文主文件
module;
#include <vulkan/vulkan.h>

#include <vector>
export module vk:vkcore;
import spdlog;
import glfw;

import defs;

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
    availableStr.resize(availableLayers.size());
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
    availableStr.resize(extensionCount);
    for (const auto& extension : availableExtensions)
    {
        availableStr.push_back(extension.extensionName);
    }
    return isSubset(availableStr, extensions);
}

namespace th
{
export class vkcore
{
  private:
    VkInstance instance_{nullptr};
    VkPhysicalDevice physicalDevice_{nullptr};
    VkDevice device_{nullptr};

    std::vector<const char*> validationLayers_ = {"VK_LAYER_KHRONOS_validation"};
    std::vector<const char*> deviceExtensions_ = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

  public:
    vkcore() = default;
    ~vkcore() = default;

    [[nodiscard]] VkDevice getDevice() const noexcept
    {
        return device_;
    }
    [[nodiscard]] VkPhysicalDevice getPhysicalDevice() const noexcept
    {
        return physicalDevice_;
    }

    vkcore(const vkcore&) = delete;
    vkcore(vkcore&& other) = default;
    vkcore& operator=(vkcore&& other) = default;
    vkcore& operator=(const vkcore&) = delete;

  private:
    operr init(const char* appName, const char* engineName, uint32_t extensionCount, const char** extensions)
    {
        spdlog::debug("开始初始化vulkan, 应用名{}, 引擎:{}", appName, engineName);
        auto e = createInstance(appName, engineName, extensionCount, extensions);
        if (e.has_value())
        {
            return e;
        }
        return {};
    }

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
};
} // namespace th