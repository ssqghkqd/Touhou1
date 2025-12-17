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
    "file not found",
    "file open failed",
    "init glfw failed",
    "create window failed",
    "load texture failed",
    "vk validation not suppoeted",
    "vk instance failed"

};

} // namespace th::defs

export using err = th::defs::ErrorType;
export using operr = std::optional<err>;
