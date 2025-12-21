// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
//
module;
#include <vulkan/vulkan.h>

#include <algorithm>
export module vk:Swapchain;
import glfw;
import spdlog;
import defs;

import :common;

namespace th::vk
{
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
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

VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
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

VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, glfw::window* window)
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

export class Swapchain
{
  private:
    VkSwapchainKHR swapchain_{nullptr};
    std::vector<VkImage> swapChainImages_{};
    VkFormat swapChainImageFormat_{};
    VkExtent2D swapChainExtent_{};
    std::vector<VkImageView> swapChainImageViews_{};
    // 依赖
    VkSurfaceKHR surface_{nullptr};
    VkDevice device_{nullptr};
    VkPhysicalDevice physicalDevice_{nullptr};

  public:
    explicit Swapchain(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface, VkDevice device)
        : surface_(surface), device_(device), physicalDevice_(physicalDevice)
    {
    }
    ~Swapchain()
    {
        if (!swapChainImageViews_.empty())
        {
            for (const auto imageView : swapChainImageViews_)
            {
                vkDestroyImageView(device_, imageView, nullptr);
            }
            swapChainImageViews_.clear();
            spdlog::debug("vk：销毁交换链图像");
        }
        if (surface_ != nullptr)
        {
            vkDestroySwapchainKHR(device_, swapchain_, nullptr);
            spdlog::debug("vk：销毁交换链");
        }
    }

    uint32_t acquireNextImage(VkSemaphore imageAvailableSemaphore) const
    {
        uint32_t imageIndex{0};
        vkAcquireNextImageKHR(device_, swapchain_, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
        return imageIndex;
    }

    void present(VkQueue presentQueue, VkSemaphore renderFinishedSemaphore, const uint32_t* imageIndex) const
    {
        const VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        const VkSwapchainKHR swapChains[] = {swapchain_};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = imageIndex;
        presentInfo.pResults = nullptr; // Optional
        vkQueuePresentKHR(presentQueue, &presentInfo);
    }

  private:
    friend class Renderer;
    operr createSwapChain(glfw::window* window)
    {
        const SwapChainSupportDetails swapChainSupport = querySwapChainSupport(physicalDevice_, surface_);

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

        QueueFamilyIndices indices = findQueueFamilies(physicalDevice_, surface_);
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

        const VkResult result = vkCreateSwapchainKHR(device_, &createInfo, nullptr, &swapchain_);
        if (result != VK_SUCCESS)
        {
            spdlog::critical("vk:交换链创建失败");
            return err::vk_swapchain_failed;
        }

        vkGetSwapchainImagesKHR(device_, swapchain_, &imageCount, nullptr);
        swapChainImages_.resize(imageCount);
        vkGetSwapchainImagesKHR(device_, swapchain_, &imageCount, swapChainImages_.data());

        swapChainExtent_ = extent;
        swapChainImageFormat_ = surfaceFormat.format;
        spdlog::debug("vk:创建交换链");
        return {};
    }
    operr createImageViews()
    {
        swapChainImageViews_.resize(swapChainImages_.size());
        for (size_t i = 0; i < swapChainImages_.size(); i++)
        {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = swapChainImages_[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = swapChainImageFormat_;

            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            const VkResult result = vkCreateImageView(device_, &createInfo, nullptr, &swapChainImageViews_[i]);
            if (result != VK_SUCCESS)
            {
                spdlog::critical("vk:图像视图创建失败");
                return err::vk_imageview_failed;
            }
        }
        spdlog::debug("vk：创建交换链图像视图");
        return {};
    }
};
} // namespace th::vk