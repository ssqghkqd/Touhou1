// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
//
module;
#include <vulkan/vulkan.h>

#include <vector>

export module vk:vkCommandBuffer;
import :common;
import spdlog;
import defs;

namespace th
{
export class vkCommandBuffer
{
  private:
    VkCommandPool commandPool_{VK_NULL_HANDLE};
    VkCommandBuffer commandBuffer_{VK_NULL_HANDLE};
    friend class vkRender;

  public:
    operr init(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
    {
        auto e = createCommandPool(device, physicalDevice, surface);
        if (e.has_value())
            return e;
        e = createCommandBuffer(device);
        if (e.has_value())
            return e;

        return {};
    }

  private:
    operr createCommandPool(VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
    {
        const QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice, surface);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool_) != VK_SUCCESS)
        {
            spdlog::critical("创建命令池失败");
            return err::vk_command_pool_failed;
        }
        return {};
    }

    operr createCommandBuffer(VkDevice device)
    {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool_;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1;

        if (vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer_) != VK_SUCCESS)
        {
            spdlog::critical("创建命令缓冲区失败");
            return err::vk_command_buffer_failed;
        }
        return {};
    }
};
} // namespace th
