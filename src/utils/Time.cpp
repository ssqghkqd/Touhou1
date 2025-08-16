#include <utils/Time.hpp>
#include <chrono>

namespace th
{
    float Time::lastFrameTime = 0.0f;
    float Time::deltaTime = 0.016f; // 默认16ms (60FPS)
    float Time::currentTime = 0.0f;
    float Time::windowTime = 0.0f;
    std::time_t Time::timeStamp = 0;

    float Time::getDeltaTime()
    {
        return deltaTime;
    }

    std::string &Time::getTimeStr()
    {
        return timestr;
    }

    float Time::getWindowTime()
    {
        return windowTime;
    }

    std::time_t &Time::getTimeStamp()
    {
        return timeStamp;
    }

    void Time::update()
    {
        // 计算dt
        currentTime = glfwGetTime(); // GLFW提供的时间函数
        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        // 获取各种时间并格式化
        windowTime = currentTime;
        auto now = std::chrono::floor<std::chrono::seconds>( // 关键：截断到秒级
            std::chrono::system_clock::now());
        timeStamp = std::chrono::system_clock::to_time_t(now);
        auto localTime = std::chrono::zoned_time{"Asia/Shanghai", now};
        timestr = std::format("{:%H:%M:%S}", localTime);
    }

} // namespace th
