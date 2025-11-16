#pragma once
#include <ctime>
#include <string>

namespace th::Time
{
double getDeltaTime();
double getTime();
void init(); // 初始化时调用
void gameStart(); // 游戏开始时调用
void update(); // 每帧调用
} // namespace th
