#pragma once

#include "entt/entt.hpp"
#include "stdafx.hpp"

namespace th
{
class BulletSystem
{
  public:
    void init(entt::registry& registry);
    static BulletSystem& getInstance();

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

  private:
    BulletSystem() = default;
    ~BulletSystem() = default;
    bool inited = false;
    float spawnTimer = 0.0f;
    float spawnInterval = 0.5f; // 每0.5秒生成一个
};
} // namespace th
