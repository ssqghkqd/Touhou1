// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
// 数学操作
module;
#include <cmath>
export module utils:Math;
import glm;

export namespace th::Maths
{
float normalizeAngle(float angle)
{
    angle = std::fmod(angle, 2 * M_PI);
    if (angle < 0.0f)
    {
        angle += 2 * M_PI;
    }
    return angle;
}

// 此函数返回归一化的前向量
glm::vec3 calculateForward(float pitch, float yaw)
{
    return {
        -std::cos(pitch) * std::sin(yaw),
        std::sin(pitch),
        -std::cos(pitch) * std::cos(yaw)};
}
} // namespace mc::Maths
