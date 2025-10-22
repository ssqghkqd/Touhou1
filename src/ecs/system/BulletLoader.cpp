#include "../../../include/ecs/system/BulletLoader.hpp"

#include "ecs/component/PlayerComponent.hpp"
#include "ecs/component/SpriteComponent.hpp"
#include "ecs/system/BulletSystem.hpp"
#include "ecs/system/PlayerSystem.hpp"
#include "entt/entity/registry.hpp"
#include "vec2.hpp"

namespace th::BulletLoader
{
void testSpawnRing(entt::registry& registry, const glm::vec2& centerPos, float radius, float speed, const int count)
{
    auto& player = PlayerSystem::getPlayer();
    const auto& tf = registry.get<SpriteComponent>(player);
    const auto& hb = registry.get<HitboxComponent>(player);
    const glm::vec2 m_playerPosition = tf.position + hb.offset;

    const double stepRadius = 2 * M_PI / count;                                             // 单步步长角度
    double angle = atan2(m_playerPosition.y - centerPos.y, m_playerPosition.x - centerPos.x); // 当前角度
    for (int i = 0; i < count; i++)
    {
        const glm::vec2 v = glm::vec2(speed * cos(angle), speed * sin(angle)); // 这颗子弹的速度
        const glm::vec2 pos = glm::vec2(centerPos.x + radius * cos(angle), centerPos.y + radius * sin(angle));
        BulletSystem::createBullet(registry, pos, v);
        angle += stepRadius;
    }
}
} // namespace th::BulletLoader