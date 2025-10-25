#pragma once
#include <ctime>
#include <string>

namespace th
{
    class Time
    {
    public:
        static float getDeltaTime();
        static float getWindowTime();
        static std::time_t &getTimeStamp();
        static std::string &getTimeStr();
        static void update();

    private:
        Time() = delete;
        ~Time() = delete;
        // 静态成员变量
        static float lastFrameTime;
        static float deltaTime;
        static float currentTime;

        // 真实时间
        static float windowTime;
        static std::time_t timeStamp;
        inline static std::string timestr;
    };

} // namespace th
