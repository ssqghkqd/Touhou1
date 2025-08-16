#pragma once
#include "stdafx.hpp"

namespace th
{
    struct BulletTag{};
    // 弹幕基本组件
    struct BulletComponent
    {
        glm::vec2 velocity = {0.0f, 0.0f}; // 速度矢量
        float speed = 0.0f;                // 速度大小
        float lifetime = 5.0f;             // 生命周期（秒）
        bool isPlayerBullet = false;       // 区分玩家/敌人弹幕
        bool isExistForever = true;       // 永远存在(直到超出边界)
    };

    // 弹幕碰撞组件（为未来碰撞系统预留）
    struct BulletCollider
    {
        float radius = 32.0f; // 碰撞半径
    };

    // 自机狙类型组件
    struct Homing
    {
        glm::vec2 targetPos = {0.0f, 0.0f}; // 可初始化为(0,0)
    };
}