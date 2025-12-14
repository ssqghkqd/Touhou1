// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
// 窗口主类
module;
#include <optional>
export module core:Window;
import glfw;
import spdlog;
import entt;

import defs;

namespace th
{

export class Window
{
  private:
    glfw::window* window_ = nullptr;
    bool isDisplayCursor_ = false;

    double lastTime_ = 0.0;
    int fps_ = 0;

  public:
    Window() = default;
    ~Window()
    {
        glfw::destroyWindow(window_);
        window_ = nullptr;
    }

    operr createWindow(int width, int height, const char* title)
    {
        if (window_ != nullptr)
        {
            spdlog::warn("窗口已经创建");
            return std::nullopt;
        }

        glfw::windowHint(glfw::client_api, glfw::no_api);
        window_ = glfw::createWindow(width, height, title, nullptr, nullptr);

        if (!window_)
        {
            spdlog::critical("窗口创建失败");
            return err::window_create_window_failed;
        }
        spdlog::debug("窗口创建成功");

        return std::nullopt;
    }

    bool shouldClose() const
    {
        return glfw::windowShouldClose(window_);
    }

    void swapBuffers() const // gl专用
    {
        glfw::swapBuffers(window_);
    }

    void pollEvents() const
    {
        glfw::pollEvents();
    }

    void closeWindow() const
    {
        glfw::setWindowShouldClose(window_, true);
    }

    void toggleCursor()
    {
        if (isDisplayCursor_)
        {
            glfw::setInputMode(window_, glfw::cursor, glfw::cursor_disabled);
            isDisplayCursor_ = false;
        }
        else
        {
            glfw::setInputMode(window_, glfw::cursor, glfw::cursor_normal);
            isDisplayCursor_ = true;
        }
        spdlog::info("鼠标模式: {}", isDisplayCursor_ ? "显示" : "隐藏");
    }

    [[nodiscard]] bool isKeyPressed(defs::Key key) const
    {
        return glfw::getKey(window_, static_cast<int>(key)) == glfw::press;
    }

    [[nodiscard]] defs::MousePos getMousePos() const
    {
        double xpos, ypos;
        glfw::getCursorPos(window_, &xpos, &ypos);
        return {.x = (float)xpos, .y = (float)ypos};
    }

    [[nodiscard]] bool isButtonPressed(defs::Button button) const
    {
        return glfw::getMouseButton(window_, static_cast<int>(button)) == glfw::press;
    }

    void updateFPS(double currentTime)
    {
        fps_++;
        if (currentTime - lastTime_ >= 1.0)
        {
            lastTime_ = currentTime;
            spdlog::info("FPS: {}", fps_);
            fps_ = 0;
        }
    }

    [[nodiscard]] glfw::window* getWindow() const
    {
        return window_;
    }

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window(Window&&) = default;
    Window& operator=(Window&&) = default;
};
} // namespace mc
