// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
//
module;

export module defs:Input;
import glfw;

namespace th::defs
{
export enum class Key
{
    w = glfw::key_w,
    a = glfw::key_a,
    s = glfw::key_s,
    d = glfw::key_d,
    space = glfw::key_space,
    left_control = glfw::key_left_control,
    left_alt = glfw::key_left_alt,
    left_shift = glfw::key_left_shift,
    ecs = glfw::key_escape
};

export struct MousePos
{
    float x = 0.0f;
    float y = 0.0f;
};

export enum class Button
{
    left = glfw::mouse_button_left,
    right = glfw::mouse_button_right,
};
}
