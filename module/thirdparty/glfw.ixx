//
// Created by ss on 2025/11/22.
//
module;
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

export module glfw;

export namespace glfw
{
auto& init = glfwInit;
auto& windowHint = glfwWindowHint;
auto& createWindow = glfwCreateWindow;
auto& makeContextCurrent = glfwMakeContextCurrent;
auto& getProcAddress = glfwGetProcAddress;
auto& swapInterval = glfwSwapInterval;
auto& destroyWindow = glfwDestroyWindow;
auto& setWindowShouldClose = glfwSetWindowShouldClose;
auto& windowShouldClose = glfwWindowShouldClose;
auto& swapBuffers = glfwSwapBuffers;
auto& pollEvents = glfwPollEvents;
auto& getKey = glfwGetKey;

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
} // namespace glfw