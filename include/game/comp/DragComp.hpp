#pragma once

namespace th
{
struct DragComp // 玩家移动时的平滑效果
{
    float smoothFactor = 0.7f; // 0.0-1.0，值越大响应越快
};
} // namespace th