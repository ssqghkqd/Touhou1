// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
//
module;
#include <vulkan/vulkan.h>
export module vk:CommandBuffer;
import defs;
import spdlog;

import :common;

namespace th::vk
{
export class CommandBuffer
{
  private:
    VkCommandPool commandPool_{nullptr};
    VkCommandBuffer commandBuffer_{nullptr};

    // 依赖
    VkDevice device_{nullptr};
    VkPhysicalDevice physicalDevice_{nullptr};
    VkSurfaceKHR surface_{nullptr};

  public:
    explicit CommandBuffer(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
        : device_(device), physicalDevice_(physicalDevice), surface_(surface)
    {
    }
    ~CommandBuffer()
    {
        if (commandBuffer_ != nullptr)
        {
            vkDestroyCommandPool(device_, commandPool_, nullptr);
            spdlog::debug("vk:销毁命令池和命令缓冲");
        }
    }

    void recordCommandBuffer(VkExtent2D extent, VkPipeline pipeline, VkFramebuffer frame, VkRenderPass renderPass) const
    {
        vkResetCommandBuffer(commandBuffer_, 0);
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;                  // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional
        vkBeginCommandBuffer(commandBuffer_, &beginInfo);

        {
            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = renderPass;
            renderPassInfo.framebuffer = frame;
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = extent;
            constexpr VkClearValue clearColor = {{{0.53f, 0.81f, 0.98f, 1.0f}}};
            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &clearColor;

            // =====================开始=========================
            vkCmdBeginRenderPass(commandBuffer_, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(commandBuffer_, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);
            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = static_cast<float>(extent.width);
            viewport.height = static_cast<float>(extent.height);
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            vkCmdSetViewport(commandBuffer_, 0, 1, &viewport);

            VkRect2D scissor{};
            scissor.offset = {0, 0};
            scissor.extent = extent;
            vkCmdSetScissor(commandBuffer_, 0, 1, &scissor);

            vkCmdDraw(commandBuffer_, 3, 1, 0, 0);
            vkCmdEndRenderPass(commandBuffer_);
        }

        vkEndCommandBuffer(commandBuffer_);
    }

  private:
    friend class Renderer;
    operr createCommandPool()
    {
        const QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice_, surface_);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        if (vkCreateCommandPool(device_, &poolInfo, nullptr, &commandPool_) != VK_SUCCESS)
        {
            spdlog::critical("vk:创建命令池失败");
            return err::vk_command_pool_failed;
        }
        spdlog::debug("vk:创建命令池");
        return {};
    }
    operr createCommandBuffer()
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool_;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(device_, &allocInfo, &commandBuffer_) != VK_SUCCESS)
        {
            spdlog::critical("vk:创建命令缓冲区失败");
            return err::vk_command_buffer_failed;
        }
        spdlog::debug("vk:创建命令缓冲");
        return {};
    }
};
} // namespace th::vk
