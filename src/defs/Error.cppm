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
};
export const char* errorStr[] = {
    "file not found",
    "file open failed",
    "init glfw failed",
    "create window failed",
    "load texture failed",
    "vk validation not suppoeted",

};

} // namespace th::defs

export using err = th::defs::ErrorType;
export using operr = std::optional<th::defs::ErrorType>;