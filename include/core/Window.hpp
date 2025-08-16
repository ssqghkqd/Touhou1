#pragma once
#include <stdafx.hpp>

namespace th
{

    class Window
    {
    public:
        static Window &getInstance();
        void init(int width, int height, const char *title);
        void shutdown();
        ~Window();

        Window(const Window &) = delete;
        Window &operator=(const Window &) = delete;
        Window(Window &&) = delete;
        Window &operator=(Window &&) = delete;

        bool shouldClose() const;
        void swapBuffers();
        void pollEvents();
        void close();

        // 判断一个键是否按下
        bool isKeyPressed(int key) const;
        bool isKeyRelease(int key) const;
        int getFPS();

    private:
        Window() = default;
        bool glfwInitialized = false;
        bool inited = false;
        GLFWwindow *m_window = nullptr;
        double m_lastTime = 0.0;
        int m_frameCount = 0;
        int m_fps = 0;
    };
}