// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
//
module;
#include <vulkan/vulkan.h>

#include <vector>
export module vk:vkRender;
import defs;
import spdlog;
import :vkcore;
import :vkSurface;
import :vkpipeline;
import :vkCommandBuffer;

namespace th
{
export class vkRender
{
  private:
    VkSemaphore imageAvailableSemaphore_{VK_NULL_HANDLE};
    VkSemaphore renderFinishedSemaphore_{VK_NULL_HANDLE};
    VkFence inFlightFence_{VK_NULL_HANDLE};

    vkcore core_{};
    vkSurface surface_{};
    vkPipeline pipeline_{};
    vkCommandBuffer commandBuffer_{};

  public:
    operr init(uint32_t extensionCount, const char** extensions, glfw::window* window)
    {
        core_.init("game", "ss engine", extensionCount, extensions);
        surface_.createSurface(core_.instance_, window);
        core_.initDevice(surface_.surface_);
        surface_.createSwapChain(window, core_.physicalDevice_, core_.device_);
        surface_.createImageViews(core_.device_);

        pipeline_.init(surface_.swapChainImageFormat_, core_.device_, surface_.swapChainExtent_, surface_.swapChainImageViews_);
        commandBuffer_.init(core_.device_, core_.physicalDevice_, surface_.surface_);
        auto e = createSyncObjects(core_.device_);
        if (e.has_value())
            return e;

        return {};
    }

    void drawFrame()
    {
        vkWaitForFences(core_.device_, 1, &inFlightFence_, VK_TRUE, UINT64_MAX);
        vkResetFences(core_.device_, 1, &inFlightFence_);

        uint32_t imageIndex{0};
        vkAcquireNextImageKHR(core_.device_, surface_.swapChain_, UINT64_MAX, imageAvailableSemaphore_, VK_NULL_HANDLE, &imageIndex);

        vkResetCommandBuffer(commandBuffer_.commandBuffer_, 0);
        recordCommandBuffer(commandBuffer_.commandBuffer_,
                            imageIndex,
                            pipeline_.renderPass_,
                            surface_.swapChainExtent_,
                            pipeline_.swapChainFramebuffers_,
                            pipeline_.graphicsPipeline_);

        // 提交
        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        const VkSemaphore waitSemaphores[] = {imageAvailableSemaphore_};
        constexpr VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer_.commandBuffer_;
        const VkSemaphore signalSemaphores[] = {renderFinishedSemaphore_};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkQueueSubmit(core_.graphicsQueue_, 1, &submitInfo, inFlightFence_);

        // 呈现
        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = {surface_.swapChain_};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional
        vkQueuePresentKHR(core_.presentQueue_, &presentInfo);
    }

  private:
    operr createSyncObjects(VkDevice device)
    {
        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphore_) != VK_SUCCESS ||
            vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphore_) != VK_SUCCESS ||
            vkCreateFence(device, &fenceInfo, nullptr, &inFlightFence_) != VK_SUCCESS)
        {
            spdlog::critical("创建同步对象失败");
            return err::vk_sync_object_failed;
        }
        return {};
    }

    void recordCommandBuffer(VkCommandBuffer commandBuffer,
                             uint32_t imageIndex,
                             VkRenderPass renderPass,
                             VkExtent2D swapChainExtent,
                             const std::vector<VkFramebuffer>& swapChainFrameBuffers,
                             VkPipeline graphicsPipeline) const
    {
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = 0;                  // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            spdlog::critical("启动命令缓冲失败");
        }

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = swapChainFrameBuffers[imageIndex];
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapChainExtent;
        constexpr VkClearValue clearColor = {{{0.53f, 0.81f, 0.98f, 1.0f}}};
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        // =====================开始=========================
        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(swapChainExtent.width);
        viewport.height = static_cast<float>(swapChainExtent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = swapChainExtent;
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

        vkCmdDraw(commandBuffer, 3, 1, 0, 0);

        vkCmdEndRenderPass(commandBuffer);

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            spdlog::critical("结束命令缓冲失败！");
        }
    }
};
} // namespace th
