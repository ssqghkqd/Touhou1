module;
#include <chrono>
module utils.Time;

namespace th::Time
{
using Clock = std::chrono::steady_clock;
using Seconds = std::chrono::duration<double>;

static Clock::time_point appStartTime; // 程序启动的绝对时间
static Clock::time_point gameStartTime; // 游戏开始的时间
static Clock::time_point lastFrameTime;
static double dt = 0.0;
static double t = 0.0;

void init()
{
    appStartTime = Clock::now();
}
void gameStart()
{
    gameStartTime = Clock::now();
    lastFrameTime = gameStartTime;
}


double getDeltaTime()
{
    return dt;
}

double getTime()
{
    return t;
}

void update()
{
    const auto now = Clock::now();
    dt = std::chrono::duration_cast<std::chrono::duration<double>>(now - lastFrameTime).count();
    t = std::chrono::duration_cast<std::chrono::duration<double>>(now - gameStartTime).count();
    lastFrameTime = now;
}

} // namespace th::Time
