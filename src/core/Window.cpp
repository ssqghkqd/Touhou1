#include <core/Window.hpp>
#include <utils/Logger.hpp>
#include "core/App.hpp"
#include <fstream>

namespace th
{

    Window &Window::getInstance()
    {
        static Window instance;
        if (!instance.inited)
            instance.init(App::width, App::height, App::title);
        return instance;
    }

    void Window::init(int width, int height, const char *title)
    {
        if (inited)
        {
            thLogger::warning("window已初始化");
            return;
        }
        std::ofstream log("debug.log");
        log << "启动程序...\n";

        if (!glfwInitialized)
        {
            if (!glfwInit())
            {
                log << "GLFW初始化失败\n";
                char buf[1024];
                glfwGetError((const char **)&buf); // 获取GLFW错误
                log << "GLFW ERROR: " << buf << "\n";
                thLogger::Log(thLogger::Critical, "GLFW初始化失败！");
            }
            glfwInitialized = true;
            thLogger::Log(thLogger::Info, "GLFW初始化成功");
        }

        if (m_window)
        {
            thLogger::Log(thLogger::Warning, "窗口已经创建！");
        }
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, 0);
        m_window = glfwCreateWindow(width, height, title, NULL, NULL);
        glfwSwapInterval(1); // 启用垂直同步（锁定显示器刷新率）

        // 检查是否成功创建窗口
        if (!m_window)
        {
            thLogger::Log(thLogger::Critical, "窗口创建失败！");
        }
        thLogger::Log(thLogger::Info, "窗口创建成功");

        glfwMakeContextCurrent(m_window);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            thLogger::Log(thLogger::Critical, "GLAD初始化失败！");
        }
        thLogger::Log(thLogger::Info, "GLAD初始化成功");
        glViewport(0, 0, width, height);
        thLogger::info("window初始化成功");
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

    void Window::close()
    {
        glfwSetWindowShouldClose(m_window, true);
    }

    bool Window::shouldClose() const
    {
        return glfwWindowShouldClose(m_window);
    }
    // 交换缓冲区
    void Window::swapBuffers()
    {
        if (!m_window)
            thLogger::Log(thLogger::Critical, "m_window是空指针！");
        glfwSwapBuffers(m_window);
    }
    // 轮询事件
    void Window::pollEvents()
    {
        if (!m_window)
            thLogger::Log(thLogger::Critical, "m_window是空指针！");
        glfwPollEvents();
    }

    // 判断按键函数
    bool Window::isKeyPressed(int key) const
    {
        if (!m_window)
            thLogger::Log(thLogger::Critical, "m_window是空指针！");
        return glfwGetKey(m_window, key) == GLFW_PRESS;
    }

    bool Window::isKeyRelease(int key) const
    {
        return glfwGetKey(m_window, key) == GLFW_RELEASE;
    }

    int Window::getFPS()
    {
        double currentTime = glfwGetTime();
        m_frameCount++;

        // 每1秒更新一次FPS
        if (currentTime - m_lastTime >= 1.0)
        {
            m_fps = m_frameCount;
            m_frameCount = 0;
            m_lastTime = currentTime;
        }

        return m_fps;
    }
}