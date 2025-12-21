// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
//
module;
#include <vulkan/vulkan.h>
export module vk:SyncObject;
import spdlog;
import defs;

namespace th::vk
{
struct FrameSyncObject
{
    VkSemaphore imageAvailableSemaphore_{nullptr};
    VkSemaphore renderFinishedSemaphore_{nullptr};
    VkFence inFlightFence_{nullptr};

    void cleanup(VkDevice device) const
    {
        if (imageAvailableSemaphore_ != nullptr)
        {
            vkDestroySemaphore(device, imageAvailableSemaphore_, nullptr);
        }
        if (renderFinishedSemaphore_ != nullptr)
        {
            vkDestroySemaphore(device, renderFinishedSemaphore_, nullptr);
        }
        if (inFlightFence_ != nullptr)
        {
            vkDestroyFence(device, inFlightFence_, nullptr);
        }
    }
};

export class SyncObject
{
  private:
    FrameSyncObject frameSyncObject_{};
    // 依赖
    VkDevice device_{nullptr};

  public:
    explicit SyncObject(VkDevice device)
        : device_(device)
    {
    }
    ~SyncObject()
    {
        frameSyncObject_.cleanup(device_);
        spdlog::debug("vk：销毁同步对象");
    }

    void waitFence() const
    {
        vkWaitForFences(device_, 1, &frameSyncObject_.inFlightFence_, VK_TRUE, UINT64_MAX);
        vkResetFences(device_, 1, &frameSyncObject_.inFlightFence_);
    }

  private:
    friend class Renderer;
    operr createSyncObjects()
    {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (vkCreateSemaphore(device_, &semaphoreInfo, nullptr, &frameSyncObject_.imageAvailableSemaphore_) != VK_SUCCESS ||
            vkCreateSemaphore(device_, &semaphoreInfo, nullptr, &frameSyncObject_.renderFinishedSemaphore_) != VK_SUCCESS ||
            vkCreateFence(device_, &fenceInfo, nullptr, &frameSyncObject_.inFlightFence_) != VK_SUCCESS)
        {
            spdlog::critical("vk:创建同步对象失败");
            return err::vk_sync_object_failed;
        }
        spdlog::debug("vk:创建同步对象");
        return {};
    }
};
} // namespace th::vk
