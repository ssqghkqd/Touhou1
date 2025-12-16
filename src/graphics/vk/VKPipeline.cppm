// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
//
module;
#include <vulkan/vulkan.h>
export module vk:VKPipeline;
import spdlog;

namespace th::vk
{
export class VKPipeline
{
  private:
  public:
    void createImageViews()
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
                spdlog::critical("图像视图创建失败");
            }
        }
    }
};
} // namespace th::vk
