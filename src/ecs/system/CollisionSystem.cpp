#include <entt/entt.hpp>
#include <glm.hpp>

#include "ecs/comp/BulletComp.hpp"
#include "ecs/comp/CollisionComp.hpp"
#include "ecs/comp/PlayerComp.hpp"
#include "ecs/comp/SpriteComp.hpp"
#include "ecs/comp/TransformComp.hpp"
#include "ecs/system/PlayerSystem.hpp"
#include "resources/AudioManager.hpp"
#include "utils/Logger.hpp"

#include "ecs/system/CollisionSystem.hpp"

namespace th::CollisionSystem
{

void update(entt::registry& registry)
{
    const auto m_player = PlayerSystem::getPlayer();

    // 获取玩家碰撞数据
    auto& sprite = registry.get<SpriteComp>(m_player);
    auto& tf = registry.get<TransformComp>(m_player);
    auto& playerc = registry.get<PlayerComp>(m_player);
    auto& cs = registry.get<CollisionComp>(m_player);


    auto m_playerPosition = tf.position + playerc.hitboxOffset;
    auto m_playerRadius = cs.radius;

    // 1.检测弹幕碰撞
    auto bulletView = registry.view<BulletComp, TransformComp, CollisionComp>();

    bulletView.each([&](entt::entity bullet, auto& bulletComp, auto& transform, auto& collision)
                    {
                        if (bulletComp.isPlayer)
                        {
                            thLogger::debug("跳过玩家弹幕");
                            return; // 忽略玩家弹幕
                        }

                        // 弹幕使用点碰撞
                        if (checkCollision(m_playerPosition, m_playerRadius, transform.position, collision.radius))
                        {
                            thLogger::info("玩家碰撞弹幕！");
                            // 播放被弹音效
                            auto& audio = AudioManager::getInstance();
                            audio.playSound("miss", 0.6f, m_playerPosition);
                            thLogger::info("播放音效");
                            registry.destroy(bullet);
                        }
                    });
}

bool checkCollision(const glm::vec2& posA, const float radiusA, const glm::vec2& posB, const float radiusB)
{
    // 正交圆判定 当距离的平方小于二者半价平方之和的时候判定 即两条半径 和圆心连线构成直角三角形的时候判定
    const float dx = posB.x - posA.x;
    const float dy = posB.y - posA.y;

    const float distanceSquared = dx * dx + dy * dy;
    const float radiiSquared = radiusA * radiusA + radiusB * radiusB;

    return distanceSquared < radiiSquared;
}

}