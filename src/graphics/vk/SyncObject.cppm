// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
//
module;
#include <vulkan/vulkan.h>

#include <array>
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
    std::array<FrameSyncObject, defs::max_frames_in_flight> framesSyncs_;
    // 依赖
    VkDevice device_{nullptr};

  public:
    explicit SyncObject(VkDevice device)
        : device_(device)
    {
    }
    ~SyncObject()
    {
        for (auto& syncObject : framesSyncs_)
        {
            syncObject.cleanup(device_);
        }
        spdlog::debug("vk：销毁同步对象");
    }

    void waitFence(size_t frameIndex) const
    {
        vkWaitForFences(device_, 1, &framesSyncs_[frameIndex].inFlightFence_, VK_TRUE, UINT64_MAX);
        vkResetFences(device_, 1, &framesSyncs_[frameIndex].inFlightFence_);
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

        for (size_t i = 0; i < defs::max_frames_in_flight; i++)
        {
            if (vkCreateSemaphore(device_, &semaphoreInfo, nullptr, &framesSyncs_[i].imageAvailableSemaphore_) != VK_SUCCESS ||
                vkCreateSemaphore(device_, &semaphoreInfo, nullptr, &framesSyncs_[i].renderFinishedSemaphore_) != VK_SUCCESS ||
                vkCreateFence(device_, &fenceInfo, nullptr, &framesSyncs_[i].inFlightFence_) != VK_SUCCESS)
            {
                spdlog::critical("vk:创建同步对象失败");
                return err::vk_sync_object_failed;
            }
        }

        spdlog::debug("vk:创建同步对象");
        return {};
    }
};
} // namespace th::vk
