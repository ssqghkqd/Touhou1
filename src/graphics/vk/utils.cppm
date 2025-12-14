// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
//
module;
#include <vulkan/vulkan.h>

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <vector>
export module vk:utils;
import spdlog;
import glfw;

namespace th::vk
{
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

export VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
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

export bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers)
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

export bool checkInstanceExtensionsSupport(const std::vector<const char*>& extensions)
{
    uint32_t extensionCount{0};
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, availableExtensions.data());
    std::vector<const char*> availableStr{};
    availableStr.reserve(availableExtensions.size());
    for (const auto& extension : availableExtensions)
    {
        availableStr.push_back(extension.extensionName);
    }
    return isSubset(availableStr, extensions);
}

export bool checkDeviceExtensionsSupport(VkPhysicalDevice physicalDevice,
                                         const std::vector<const char*>& extensions)
{
    uint32_t extensionCount{0};
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());
    std::vector<const char*> availableStr{};
    availableStr.reserve(availableExtensions.size());
    for (const auto& extension : availableExtensions)
    {
        availableStr.push_back(extension.extensionName);
    }
    return isSubset(availableStr, extensions);
}

// =================================== 交换链相关==========================
export VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
{
    for (const auto& availableFormat : availableFormats)
    {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
            availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormat;
        }
    }
    return availableFormats[0];
}

export VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
{
    for (const auto& availablePresentMode : availablePresentModes)
    {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            return availablePresentMode;
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

export VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, glfw::window* window)
{
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        return capabilities.currentExtent;
    }
    else
    {
        int width, height;
        glfw::getFramebufferSize(window, &width, &height);

        VkExtent2D actualExtent = {
            static_cast<uint32_t>(width),
            static_cast<uint32_t>(height)};

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return actualExtent;
    }
}

} // namespace th::vk
