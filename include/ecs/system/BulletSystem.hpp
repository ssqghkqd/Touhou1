#pragma once

#include "entt/entt.hpp"

namespace th::BulletSystem
{

inline bool inited = false;
inline float spawnTimer = 0.0f;
inline float spawnInterval = 0.5f; // 每0.5秒生成一个

void init(entt::registry& registry);

// 更新弹幕运动状态
void update(entt::registry& registry, float deltaTime);

// 创建动态弹幕（预留接口）
entt::entity createBullet(entt::registry& registry,
                          const glm::vec2& position,
                          const glm::vec2& velocity,
                          bool isPlayerBullet = false,
                          bool isExistForever = true);

// 批量创建弹幕（预留接口）
void createBulletPattern(entt::registry& registry,
                         const glm::vec2& origin,
                         int count,
                         float baseAngle,
                         float angleStep,
                         float speed,
                         bool isPlayerBullet = false);

// 测试函数随机弹幕
void spawnRandomBullet(entt::registry& registry);
} // namespace th::BulletSystem
