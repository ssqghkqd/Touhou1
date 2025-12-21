// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
//
module;
#include <vulkan/vulkan.h>

#include <memory>

#include "defs/Macro.hpp"
export module vk:Renderer;
import :Core;
import :Surface;
import :Device;
import :Swapchain;
import :Pipeline;
import :CommandBuffer;
import :SyncObject;

import spdlog;
import glfw;
import defs;

namespace th::vk
{
export class Renderer
{
  private:
    std::unique_ptr<Core> core_{};
    std::unique_ptr<Surface> surface_{};
    std::unique_ptr<Device> device_{};
    std::unique_ptr<Swapchain> swapchain_{};
    std::unique_ptr<Pipeline> pipeline_{};
    std::unique_ptr<CommandBuffer> commandBuffer_{};
    std::unique_ptr<SyncObject> syncObject_{};

  public:
    Renderer() = default;
    ~Renderer()
    {
        device_->waitIdle();

        syncObject_.reset();
        commandBuffer_.reset();
        pipeline_.reset();
        swapchain_.reset();
        device_.reset();
        surface_.reset();
        core_.reset();
    }
    operr init(uint32_t extensionCount, const char** extensions, glfw::window* window)
    {
        core_ = std::make_unique<Core>();
        auto e = core_->createInstance("Touhou1", "ss engine2D", extensionCount, extensions);
        CHECK(e);
        core_->initDebugMessenger();

        surface_ = std::make_unique<Surface>(core_->instance_);
        e = surface_->createSurface(window);
        CHECK(e);

        device_ = std::make_unique<Device>(surface_->surface_, core_->instance_);
        e = device_->pickPhysicalDevice();
        CHECK(e);
        e = device_->createLogicalDevice();
        CHECK(e);

        swapchain_ = std::make_unique<Swapchain>(device_->physicalDevice_, surface_->surface_, device_->device_);
        e = swapchain_->createSwapChain(window);
        CHECK(e);
        e = swapchain_->createImageViews();
        CHECK(e);

        pipeline_ = std::make_unique<Pipeline>(device_->device_);
        e = pipeline_->createRenderPass(swapchain_->swapChainImageFormat_);
        CHECK(e);
        e = pipeline_->createPipelineLayout();
        CHECK(e);
        e = pipeline_->createGraphicsPipeline(swapchain_->swapChainExtent_);
        CHECK(e);
        e = pipeline_->createFramebuffers(swapchain_->swapChainExtent_, swapchain_->swapChainImageViews_);
        CHECK(e);

        commandBuffer_ = std::make_unique<CommandBuffer>(device_->device_, device_->physicalDevice_, surface_->surface_);
        e = commandBuffer_->createCommandPool();
        CHECK(e);
        e = commandBuffer_->createCommandBuffer();
        CHECK(e);

        syncObject_ = std::make_unique<SyncObject>(device_->device_);
        e = syncObject_->createSyncObjects();
        CHECK(e);

        return {};
    }

    void render() const
    {
        syncObject_->waitFence();
        const auto imageIndex = swapchain_->acquireNextImage(syncObject_->frameSyncObject_.imageAvailableSemaphore_);

        commandBuffer_->recordCommandBuffer(swapchain_->swapChainExtent_,
                                            pipeline_->graphicsPipeline_,
                                            pipeline_->swapChainFramebuffers_[imageIndex],
                                            pipeline_->renderPass_);
        submitGraphicsQueue(device_->graphicsQueue_,
                            &commandBuffer_->commandBuffer_,
                            syncObject_->frameSyncObject_.imageAvailableSemaphore_,
                            syncObject_->frameSyncObject_.renderFinishedSemaphore_,
                            syncObject_->frameSyncObject_.inFlightFence_);

        swapchain_->present(device_->presentQueue_,
                            syncObject_->frameSyncObject_.renderFinishedSemaphore_,
                            &imageIndex);
    }

  private:
    static void submitGraphicsQueue(VkQueue graphicsQueue,
                                    const VkCommandBuffer* commandBuffer,
                                    VkSemaphore imageAvailableSemaphore,
                                    VkSemaphore renderFinishedSemaphore,
                                    VkFence inFlightFence)
    {
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        const VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
        constexpr VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = commandBuffer;
        const VkSemaphore signalSemaphores[] = {renderFinishedSemaphore};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFence);
    }
};
} // namespace th::vk