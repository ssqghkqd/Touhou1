// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
// 时间工具
module;
#include <chrono>
export module utils:Time;

namespace th::Time
{

using Clock = std::chrono::steady_clock;
using Seconds = std::chrono::duration<double>;

static Clock::time_point appStartTime;  // 程序启动的绝对时间
static Clock::time_point gameStartTime; // 游戏开始的时间
static Clock::time_point lastFrameTime;
static double dt = 0.0;
static double t = 0.0;

export void init()
{
    appStartTime = Clock::now();
}
export void gameStart()
{
    gameStartTime = Clock::now();
    lastFrameTime = gameStartTime;
}

export double getDeltaTime()
{
    return dt;
}

export double getTime()
{
    return t;
}

export void update()
{
    const auto now = Clock::now();
    dt = std::chrono::duration_cast<std::chrono::duration<double>>(now - lastFrameTime).count();
    t = std::chrono::duration_cast<std::chrono::duration<double>>(now - gameStartTime).count();
    lastFrameTime = now;
}

} // namespace mc::Time
