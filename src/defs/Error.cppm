// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
//
module;
#include <optional>
export module defs:Error;

namespace th::defs
{
export enum class ErrorType
{
    file_not_found,
    file_open_failed,

    init_glfw_failed,

    window_create_window_failed,

    texture_load_failed,

    vk_validation_not_supported,
    vk_instance_failed,
    vk_instance_extensions_failed,
    vk_surface_failed,
    vk_physical_device_failed,
    vk_device_failed,
    vk_swapchain_failed,
    vk_imageview_failed,
    vk_renderpass_failed,
    vk_pipeline_layout_failed,
    vk_shader_create_failed,
    vk_pipeline_failed,
    vk_framebuffer_failed,
    vk_command_pool_failed,
    vk_command_buffer_failed,
    vk_sync_object_failed,
};
export const char* errorStr[] = {
    // 文件相关错误
    "File not found",
    "Failed to open file",

    // GLFW/窗口相关错误
    "Failed to initialize GLFW",
    "Failed to create window",

    // 纹理错误
    "Failed to load texture",

    // Vulkan 初始化错误
    "Vulkan validation layers not supported",
    "Failed to create Vulkan instance",
    "Required Vulkan instance extensions not available",
    "Failed to create window surface",
    "No suitable Vulkan physical device found",
    "Failed to create Vulkan logical device",
    "Failed to create swap chain",
    "Failed to create image views",
    "Failed to create render pass",
    "Failed to create pipeline layout",
    "Failed to create shader module",
    "Failed to create graphics pipeline",
    "Failed to create framebuffers",
    "Failed to create command pool",
    "Failed to allocate command buffer",
    "Failed to create synchronization objects"

};

} // namespace th::defs

export using err = th::defs::ErrorType;
export using operr = std::optional<err>;
