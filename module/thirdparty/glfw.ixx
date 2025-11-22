//
// Created by ss on 2025/11/22.
//
module;
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

export module glfw;

export namespace glfw
{
auto init = glfwInit;
auto windowHint = glfwWindowHint;
auto createWindow = glfwCreateWindow;
auto makeContextCurrent = glfwMakeContextCurrent;
auto getProcAddress = glfwGetProcAddress;
auto swapInterval = glfwSwapInterval;
} // namespace glfw