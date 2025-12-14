// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
// glfw包装
module;
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
export module glfw;

export namespace glfw
{
auto& init = glfwInit;
auto& windowHint = glfwWindowHint;
auto& createWindow = glfwCreateWindow;
auto& getProcAddress = glfwGetProcAddress;
auto& swapInterval = glfwSwapInterval;
auto& destroyWindow = glfwDestroyWindow;
auto& setWindowShouldClose = glfwSetWindowShouldClose;
auto& windowShouldClose = glfwWindowShouldClose;
auto& swapBuffers = glfwSwapBuffers;
auto& pollEvents = glfwPollEvents;
auto& getKey = glfwGetKey;
auto& setCursorPosCallback = glfwSetCursorPosCallback;
auto& setInputMode = glfwSetInputMode;
auto& setMouseButtonCallback = glfwSetMouseButtonCallback;
auto& setWindowSizeCallback = glfwSetWindowSizeCallback;
auto& setWindowUserPointer = glfwSetWindowUserPointer;
auto& getWindowUserPointer = glfwGetWindowUserPointer;
auto& getCursorPos = glfwGetCursorPos;
auto& getMouseButton = glfwGetMouseButton;
auto& getRequiredInstanceExtensions = glfwGetRequiredInstanceExtensions;
auto& createWindowSurface = glfwCreateWindowSurface;
auto& getFramebufferSize = glfwGetFramebufferSize;

using window = GLFWwindow;

constexpr auto scale_to_monitor = GLFW_SCALE_TO_MONITOR;
constexpr auto FALSE = GLFW_FALSE;
constexpr auto TRUE = GLFW_TRUE;
constexpr auto scale_framebuffer = GLFW_SCALE_FRAMEBUFFER;
constexpr auto context_version_major = GLFW_CONTEXT_VERSION_MAJOR;
constexpr auto context_version_minor = GLFW_CONTEXT_VERSION_MINOR;
constexpr auto opengl_profile = GLFW_OPENGL_PROFILE;
constexpr auto opengl_core_profile = GLFW_OPENGL_CORE_PROFILE;
constexpr auto resizable = GLFW_RESIZABLE;
constexpr auto press = GLFW_PRESS;
constexpr auto release = GLFW_RELEASE;
constexpr auto key_escape = GLFW_KEY_ESCAPE;
constexpr auto key_z = GLFW_KEY_Z;
constexpr auto key_left_shift = GLFW_KEY_LEFT_SHIFT;
constexpr auto key_up = GLFW_KEY_UP;
constexpr auto key_right = GLFW_KEY_RIGHT;
constexpr auto key_left = GLFW_KEY_LEFT;
constexpr auto key_down = GLFW_KEY_DOWN;
constexpr auto key_w = GLFW_KEY_W;
constexpr auto key_a = GLFW_KEY_A;
constexpr auto key_s = GLFW_KEY_S;
constexpr auto key_d = GLFW_KEY_D;
constexpr auto key_space = GLFW_KEY_SPACE;
constexpr auto cursor = GLFW_CURSOR;
constexpr auto cursor_disabled = GLFW_CURSOR_DISABLED;
constexpr auto cursor_normal = GLFW_CURSOR_NORMAL;
constexpr auto key_left_alt = GLFW_KEY_LEFT_ALT;
constexpr auto mouse_button_left = GLFW_MOUSE_BUTTON_LEFT;
constexpr auto mouse_button_right = GLFW_MOUSE_BUTTON_RIGHT;
constexpr auto key_left_control = GLFW_KEY_LEFT_CONTROL;
constexpr auto client_api = GLFW_CLIENT_API;
constexpr auto no_api = GLFW_NO_API;
}
