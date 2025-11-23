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

constexpr auto SCALE_TO_MONITOR = GLFW_SCALE_TO_MONITOR;
constexpr auto FALSE = GLFW_FALSE;
constexpr auto TRUE = GLFW_TRUE;
constexpr auto SCALE_FRAMEBUFFER = GLFW_SCALE_FRAMEBUFFER;
constexpr auto CONTEXT_VERSION_MAJOR = GLFW_CONTEXT_VERSION_MAJOR;
constexpr auto CONTEXT_VERSION_MINOR = GLFW_CONTEXT_VERSION_MINOR;
constexpr auto OPENGL_PROFILE = GLFW_OPENGL_PROFILE;
constexpr auto OPENGL_CORE_PROFILE = GLFW_OPENGL_CORE_PROFILE;
constexpr auto RESIZABLE = GLFW_RESIZABLE;
constexpr auto PRESS = GLFW_PRESS;
constexpr auto RELEASE = GLFW_RELEASE;
constexpr auto KEY_ESCAPE = GLFW_KEY_ESCAPE;
constexpr auto KEY_Z = GLFW_KEY_Z;
constexpr auto KEY_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT;
constexpr auto KEY_UP = GLFW_KEY_UP;
constexpr auto KEY_RIGHT = GLFW_KEY_RIGHT;
constexpr auto KEY_LEFT = GLFW_KEY_LEFT;
constexpr auto KEY_DOWN = GLFW_KEY_DOWN;
} // namespace glfw