#include <core/Window.hpp>

#include "core/App.hpp"
#include "glad.h"
#include "spdlog/spdlog.h"
#include "utils/Time.hpp"

namespace th
{

Window::Window()
{
    init(App::width, App::height, App::title);
}


void Window::init(int width, int height, const char* title)
{
    if (inited)
    {
        spdlog::warn("window已经初始化");
        return;
    }
    App::scale = 1;

    if (!glfwInitialized)
    {
        if (!glfwInit())
        {
            spdlog::critical("GLFW初始化失败");
            throw;
        }
        glfwInitialized = true;
        spdlog::info("GLFW 初始化成功");
    }

    if (m_window)
    {
        spdlog::warn("窗口已经创建");
    }
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE);  // 禁用系统缩放
    glfwWindowHint(GLFW_SCALE_FRAMEBUFFER, GLFW_FALSE); // 禁用帧缓冲缩放
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, 0);
    m_window = glfwCreateWindow(width * App::scale, height * App::scale, title, nullptr, nullptr);

    // 检查是否成功创建窗口
    if (!m_window)
    {
        spdlog::critical("窗口创建失败");
        throw;
    }

    spdlog::info("窗口创建成功");

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        spdlog::critical("GLAD初始化失败");
        throw;
    }
    spdlog::info("GLAD初始化成功");
    glViewport(0, 0, width * App::scale, height * App::scale);
    glfwSwapInterval(0); // 启用垂直同步（锁定显示器刷新率）
    spdlog::info("window初始化成功");
    inited = true;
}

Window::~Window()
{
    shutdown();
}

void Window::shutdown()
{
    if (m_window)
    {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
}

void Window::updateFPS()
{
    const double currentTime = Time::getTime();
    m_frameCount++;

    if (currentTime - m_lastTime >= App::STAT_INTERVAL)
    {
        m_lastTime = currentTime;
        m_fps = m_frameCount;
        m_frameCount = 0;

        spdlog::info("FPS: {}, t: {:.2f}", m_fps, currentTime);
    }
}


void Window::close() const
{
    glfwSetWindowShouldClose(m_window, true);
}

bool Window::shouldClose() const
{
    return glfwWindowShouldClose(m_window);
}
// 交换缓冲区
void Window::swapBuffers() const
{
    glfwSwapBuffers(m_window);
}
// 轮询事件
void Window::pollEvents() const
{
    glfwPollEvents();
}

// 判断按键函数
bool Window::isKeyPressed(int key) const
{

    return glfwGetKey(m_window, key) == GLFW_PRESS;
}

bool Window::isKeyRelease(int key) const
{
    return glfwGetKey(m_window, key) == GLFW_RELEASE;
}
} // namespace th