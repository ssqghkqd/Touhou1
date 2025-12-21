// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
//
module;
#include <vulkan/vulkan.h>
export module vk:Surface;
import defs;
import spdlog;
import glfw;

namespace th::vk
{
export class Surface
{
  private:
    VkSurfaceKHR surface_{nullptr};

    VkInstance instance_{nullptr};

  public:
    explicit Surface(VkInstance instance)
        : instance_(instance)
    {
    }
    ~Surface()
    {
        if (surface_ != nullptr)
        {
            vkDestroySurfaceKHR(instance_, surface_, nullptr);
            spdlog::debug("vk：销毁表面");
        }
    }

  private:
    friend class Renderer;
    operr createSurface(glfw::window* window)
    {
        const VkResult result = glfw::createWindowSurface(instance_, window, nullptr, &surface_);
        if (result != VK_SUCCESS)
        {
            spdlog::critical("vk:创建表面失败");
            return err::vk_surface_failed;
        }
        spdlog::debug("vk：创建表面");
        return {};
    }
};
} // namespace th::vk
