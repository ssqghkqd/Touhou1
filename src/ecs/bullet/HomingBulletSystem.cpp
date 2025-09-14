#include "ecs/bullet/HomingBulletSystem.hpp"

#include <cmath>

#include "core/App.hpp"
#include "ecs/BulletSystem.hpp"
#include "ecs/component/PlayerComponent.hpp"
#include "ecs/component/SpriteComponent.hpp"

namespace th
{
HomingBulletSystem& HomingBulletSystem::getInstance()
{
    static HomingBulletSystem instance;
    return instance;
}

void HomingBulletSystem::init(entt::registry& registry)
{
    if (inited)
    {
        thLogger::warning("homingbulletsystem已初始化");
        return;
    }

    // 这里添加初始化过程

    // 查找玩家实体(其实一般只有一个)
    auto playerView = registry.view<PlayerTag>();
    if (!playerView.empty())
    {
        m_player = playerView.front();
    }

    thLogger::info("homingbulletsystem初始化");
    inited = true;
}

// angle为角度制, way为路数
void HomingBulletSystem::createHoming(entt::registry& registry, int way, float angle)
{
    angle = glm::radians(angle);
    // 如果没有玩家，直接返回
    if (m_player == entt::null || !registry.valid(m_player))
    {
        thLogger::warning("玩家实体无效");
        return;
    }

    // 获取资源
    auto& bs = BulletSystem::getInstance();
    // 初始生成位置
    constexpr float x = App::bgwidth * 0.5f;
    constexpr float y = App::bgoffsetY + 50.0f;
    constexpr const float speed = 300.0f;
    // 速度指向玩家
    // 获取玩家碰撞数据
    auto& tf = registry.get<TransformComponent>(m_player);
    auto& hb = registry.get<HitboxComponent>(m_player);

    m_playerPosition = tf.position + hb.offset;

    // 基础方向 单位矢量
    glm::vec2 toPlayer = glm::normalize(m_playerPosition - glm::vec2(x, y));
    if (way == 1)
    {
        bs.createBullet(registry, {x, y}, speed * toPlayer);
        return;
    }

    // 计算基础方向角度
    float toPlayerAngle = atan2(toPlayer.y, toPlayer.x);
    float startAngle = toPlayerAngle - angle * 0.5f;

    // 计算角度步长（对称分布）
    float angleStep = angle / (way - 1);
    for (int i = 0; i < way; i++)
    {
        float currentAngle = startAngle + angleStep * i;
        glm::vec2 dir(cos(currentAngle), sin(currentAngle));
        bs.createBullet(registry, {x, y}, speed * dir);
    }
}
} // namespace th
