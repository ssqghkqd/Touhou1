// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
//
module;
#include <vulkan/vulkan.h>
export module vk:vkSurface;
import glfw;
import spdlog;
import defs;

namespace th
{
export class vkSurface
{
  private:
    VkSurfaceKHR surface_{nullptr};

  public:
    operr init(VkInstance instance, glfw::window* window)
    {
        auto e = createSurface(instance, window);
        if (e.has_value())
            return e;
        spdlog::debug("vk：创建表面");
        return {};
    }

    [[nodiscard]] VkSurfaceKHR getSurface() const
    {
        return surface_;
    }

  private:
    operr createSurface(VkInstance instance, glfw::window* window)
    {
        const VkResult result = glfw::createWindowSurface(instance, window, nullptr, &surface_);
        if (result != VK_SUCCESS)
        {
            spdlog::critical("创建表面失败");
            return err::vk_surface_failed;
        }
        return {};
    }
};
} // namespace th