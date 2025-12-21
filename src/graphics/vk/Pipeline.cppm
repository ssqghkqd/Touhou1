// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
//
module;
#include <vulkan/vulkan.h>

#include <cassert>
#include <expected>
#include <vector>

export module vk:Pipeline;
import spdlog;
import defs;

import utils;

namespace th::vk
{
VkPipelineVertexInputStateCreateInfo createVertexInputState()
{
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 0;
    vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
    vertexInputInfo.vertexAttributeDescriptionCount = 0;
    vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

    return vertexInputInfo;
}
VkPipelineInputAssemblyStateCreateInfo createInputAssembly()
{
    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    return inputAssembly;
}
VkViewport createViewport(VkExtent2D swapChainExtent)
{
    VkViewport viewport{};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)swapChainExtent.width;
    viewport.height = (float)swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    return viewport;
}
VkRect2D createScissor(VkExtent2D swapChainExtent)
{
    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;
    return scissor;
}
VkPipelineViewportStateCreateInfo createViewportInfo(const VkViewport& viewport, const VkRect2D& scissor)
{
    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    return viewportState;
}
VkPipelineRasterizationStateCreateInfo createRasterizer()
{
    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;         // 不夹紧深度
    rasterizer.rasterizerDiscardEnable = VK_FALSE;  // 不丢弃图元
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;  // 填充模式
    rasterizer.lineWidth = 1.0f;                    // 线宽
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;    // 背面剔除
    rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE; // 顺时针为正面
    rasterizer.depthBiasEnable = VK_FALSE;          // 不启用深度偏移
    rasterizer.depthBiasConstantFactor = 0.0f;
    rasterizer.depthBiasClamp = 0.0f;
    rasterizer.depthBiasSlopeFactor = 0.0f;

    return rasterizer;
}
VkPipelineMultisampleStateCreateInfo createMultisample()
{
    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisampling.minSampleShading = 1.0f;          // Optional
    multisampling.pSampleMask = nullptr;            // Optional
    multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
    multisampling.alphaToOneEnable = VK_FALSE;      // Optional
    return multisampling;
}
VkPipelineColorBlendAttachmentState createColorBlendAttachment()
{
    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask =
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = VK_FALSE; // 不混合
    colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
    colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
    colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    return colorBlendAttachment;
}
VkPipelineColorBlendStateCreateInfo createColorBlend(const VkPipelineColorBlendAttachmentState& colorBlendAttachment)
{
    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE; // 不启用逻辑操作
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    return colorBlending;
}

export class Pipeline
{
  private:
    std::vector<VkDynamicState> dynamicStates_{
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR};
    VkRenderPass renderPass_{nullptr};
    VkPipelineLayout pipelineLayout_{nullptr};
    VkPipeline graphicsPipeline_{nullptr};
    std::vector<VkFramebuffer> swapChainFramebuffers_{};

    // 依赖
    VkDevice device_{nullptr};

  public:
    explicit Pipeline(VkDevice device)
        : device_(device)
    {
        assert(device != nullptr && "设备为空");
    }
    ~Pipeline()
    {
        if (!swapChainFramebuffers_.empty())
        {
            for (const auto framebuffer : swapChainFramebuffers_)
            {
                vkDestroyFramebuffer(device_, framebuffer, nullptr);
            }
            swapChainFramebuffers_.clear();
            spdlog::debug("vk:销毁帧缓冲区");
        }
        if (graphicsPipeline_ != nullptr)
        {
            vkDestroyPipeline(device_, graphicsPipeline_, nullptr);
            spdlog::debug("vk:销毁渲染管线");
        }
        if (pipelineLayout_ != nullptr)
        {
            vkDestroyPipelineLayout(device_, pipelineLayout_, nullptr);
            spdlog::debug("vk:销毁管线布局");
        }
        if (renderPass_ != nullptr)
        {
            vkDestroyRenderPass(device_, renderPass_, nullptr);
            spdlog::debug("vk:销毁渲染流程");
        }
    }

  private:
    friend class Renderer;
    operr createRenderPass(VkFormat swapChainImageFormat)
    {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = swapChainImageFormat;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        VkRenderPassCreateInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (vkCreateRenderPass(device_, &renderPassInfo, nullptr, &renderPass_) != VK_SUCCESS)
        {
            spdlog::critical("vk:创建渲染流程失败");
            return err::vk_renderpass_failed;
        }
        spdlog::debug("vk：创建渲染流程");
        return {};
    }
    operr createPipelineLayout()
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;            // Optional
        pipelineLayoutInfo.pSetLayouts = nullptr;         // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 0;    // Optional
        pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

        if (vkCreatePipelineLayout(device_, &pipelineLayoutInfo, nullptr, &pipelineLayout_) != VK_SUCCESS)
        {
            spdlog::critical("vk:创建渲染管线布局失败");
            return err::vk_renderpass_failed;
        }

        spdlog::debug("vk：创建渲染管线布局");
        return {};
    }

    operr createGraphicsPipeline(VkExtent2D swapChainExtent)
    {
        const auto vertPathPoss = FileUtils::getResourcePath("shaders/vert.spv", false);
        const auto fragPathPoss = FileUtils::getResourcePath("shaders/frag.spv", false);

        if (!vertPathPoss.has_value())
        {
            spdlog::critical("vk:顶点着色器文件不存在");
            return vertPathPoss.error();
        }
        if (!fragPathPoss.has_value())
        {
            spdlog::critical("vk:片段着色器文件不存在");
            return fragPathPoss.error();
        }

        const auto vertShaderCodePoss = FileUtils::readFileToBytes(vertPathPoss.value());
        const auto fragShaderCodePoss = FileUtils::readFileToBytes(fragPathPoss.value());

        if (!vertShaderCodePoss.has_value())
        {
            spdlog::critical("vk:无法读取顶点着色器文件");
            return vertShaderCodePoss.error();
        }
        if (!fragShaderCodePoss.has_value())
        {
            spdlog::critical("vk：无法读取片段着色器文件");
            return fragShaderCodePoss.error();
        }

        const auto vertShaderModulePoss = createShaderModule(vertShaderCodePoss.value());
        const auto fragShaderModulePoss = createShaderModule(fragShaderCodePoss.value());

        if (!vertShaderModulePoss.has_value())
        {
            spdlog::critical("vk:顶点着色器创建失败");
            return vertShaderModulePoss.error();
        }
        if (!fragShaderModulePoss.has_value())
        {
            spdlog::critical("vk：片段着色器创建失败");
            return fragShaderModulePoss.error();
        }

        const auto vertShaderModule = vertShaderModulePoss.value();
        const auto fragShaderModule = fragShaderModulePoss.value();

        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = {vertShaderStageInfo, fragShaderStageInfo};

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;

        const auto vertexInputInfo = createVertexInputState();
        const auto inputAssembly = createInputAssembly();
        const auto viewportState = createViewportInfo(createViewport(swapChainExtent), createScissor(swapChainExtent));
        const auto rasterizer = createRasterizer();
        const auto multisampling = createMultisample();
        const auto colorBlending = createColorBlend(createColorBlendAttachment());
        const auto dynamicState = createDynamicState();

        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = nullptr; // Optional
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = &dynamicState;

        pipelineInfo.layout = pipelineLayout_;
        pipelineInfo.renderPass = renderPass_;
        pipelineInfo.subpass = 0;

        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipelineInfo.basePipelineIndex = -1;              // Optional
        /*实际上还有两个参数：basePipelineHandle 和 basePipelineIndex。Vulkan 允许您通过派生自现有管线来创建新的图形管线。管线派生的思想是，当它们与现有管线具有许多共同功能时，设置管线的成本较低，并且在同一父级管线之间切换也可以更快地完成。您可以使用 basePipelineHandle 指定现有管线的句柄，也可以使用 basePipelineIndex 引用即将创建的另一个管线的索引。目前只有一个管线，所以我们只需指定一个空句柄和一个无效索引。只有在 VkGraphicsPipelineCreateInfo 的 flags 字段中也指定了 VK_PIPELINE_CREATE_DERIVATIVE_BIT 标志时，才会使用这些值。*/

        if (vkCreateGraphicsPipelines(device_,
                                      VK_NULL_HANDLE,
                                      1,
                                      &pipelineInfo,
                                      nullptr,
                                      &graphicsPipeline_) != VK_SUCCESS)
        {
            spdlog::critical("vk:创建渲染管线失败");
            return err::vk_pipeline_failed;
        }
        vkDestroyShaderModule(device_, vertShaderModule, nullptr);
        vkDestroyShaderModule(device_, fragShaderModule, nullptr);

        spdlog::debug("vk：创建渲染管线");
        return {};
    }

    operr createFramebuffers(
        VkExtent2D swapChainExtent,
        const std::vector<VkImageView>& swapChainImageViews)
    {
        swapChainFramebuffers_.resize(swapChainImageViews.size());
        for (size_t i = 0; i < swapChainImageViews.size(); i++)
        {
            const VkImageView attachments[] = {
                swapChainImageViews[i]};

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass_;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = swapChainExtent.width;
            framebufferInfo.height = swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(device_, &framebufferInfo, nullptr, &swapChainFramebuffers_[i]) != VK_SUCCESS)
            {
                spdlog::critical("vk:创建帧缓冲区失败");
                return err::vk_framebuffer_failed;
            }
        }
        spdlog::debug("vk:创建帧缓冲区");
        return {};
    }

    std::expected<VkShaderModule, err> createShaderModule(const std::vector<char>& code) const
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        VkShaderModule shaderModule;
        if (vkCreateShaderModule(device_, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            spdlog::critical("vk:创建着色器模块失败");
            return std::unexpected(err::vk_shader_create_failed);
        }
        spdlog::debug("vk：创建着色器模块");
        return shaderModule;
    }
    VkPipelineDynamicStateCreateInfo createDynamicState() const
    {
        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates_.size());
        dynamicState.pDynamicStates = dynamicStates_.data();

        return dynamicState;
    }
};
} // namespace th::vk
