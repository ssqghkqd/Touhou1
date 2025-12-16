// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
// 初始化 纯过程函数
module;
#include <cstdint>
#include <optional>
export module core:Init;
import entt;
import spdlog;
import glfw;

import utils;
import defs;

import :InputSystem;
import :Window;
import :AppLogicSys;

import vk;

namespace th::Init
{
void initBasic()
{
    Time::init();
    spdlog::set_pattern("[%H:%M:%S][%l] %v");
    spdlog::set_level(spdlog::level_enum::debug);
}

void emplaceManager(entt::registry& reg)
{
    reg.ctx().emplace<entt::dispatcher>();
    // 1. core
    reg.ctx().emplace<Window>();
    // 2. graphics
    reg.ctx().emplace<vk::vkcore>();
}

void registerEvents(entt::registry& reg)
{
    auto& dp = reg.ctx().get<entt::dispatcher>();

    AppLogic::registerHandle(dp);
}

operr createWindow(entt::registry& reg)
{
    auto& window = reg.ctx().get<Window>();
    const auto e = window.createWindow(defs::window_width * defs::window_scale,
                                       defs::window_height * defs::window_scale,
                                       defs::window_title);
    return e;
}

operr initGLFW()
{
    if (!glfw::init())
    {
        return err::init_glfw_failed;
    }
    return std::nullopt;
}

operr initVK(entt::registry& reg)
{
    auto& vkc = reg.ctx().get<vk::vkcore>();
    const auto& window = reg.ctx().get<Window>();

    uint32_t glfwExtensionsCount{0};
    const char** glfwExtensions = glfw::getRequiredInstanceExtensions(&glfwExtensionsCount);
    const auto ev = vkc.init(glfwExtensionsCount, glfwExtensions, window.getWindow());
    if (ev.has_value())
    {
        return ev;
    }
    return std::nullopt;
}

export operr
init(entt::registry& reg)
{
    initBasic();
    initGLFW();
    emplaceManager(reg);
    auto e = createWindow(reg);
    if (e.has_value())
    {
        return e;
    }
    e = initVK(reg);
    if (e.has_value())
    {
        return e;
    }


    registerEvents(reg);

    return std::nullopt;
}
} // namespace th::Init
