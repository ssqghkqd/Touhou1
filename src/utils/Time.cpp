#include <utils/Time.hpp>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace th::Time
{
    static double lastFrameTime = 0.0f;
    static double deltaTime = 0.016f; // 默认16ms (60FPS)
    static double currentTime = 0.0f;
    static double windowTime = 0.0f;

    double getDeltaTime()
    {
        return deltaTime;
    }

    double getWindowTime()
    {
        return windowTime;
    }


    void update()
    {
        // 计算dt
        currentTime = glfwGetTime(); // GLFW提供的时间函数
        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        // 获取各种时间并格式化
        windowTime = currentTime;
    }

} // namespace th
