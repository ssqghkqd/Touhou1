#include "../../../include/ecs/system_old/CollisionSystem.hpp"

#include "ecs/component_old/BulletComponent.hpp"
#include "ecs/component_old/PlayerComponent.hpp"
#include "ecs/component_old/SpriteComponent.hpp"
#include "resources/AudioManager.hpp"

namespace th::CollisionSystem
{

void init(entt::registry& registry)
{
    if (inited)
    {
        thLogger::warning("碰撞系统已初始化");
        return;
    }
    // 查找玩家实体(其实一般只有一个)
    auto playerView = registry.view<PlayerTag>();
    if (!playerView.empty())
    {
        m_player = playerView.front();
    }
    else
    {
        thLogger::error("玩家不存在");
    }
    thLogger::info("碰撞系统初始化完成");
    inited = true;
}

void update(entt::registry& registry)
{
    // 如果没有玩家，直接返回
    if (m_player == entt::null || !registry.valid(m_player))
    {
        thLogger::warning("玩家实体无效");
        return;
    }

    // 确保玩家有必要的组件
    if (!registry.all_of<SpriteComponent, HitboxComponent>(m_player))
    {
        thLogger::error("玩家缺少必要组件");
        return;
    }

    // 获取玩家碰撞数据
    auto& tf = registry.get<SpriteComponent>(m_player);
    auto& hb = registry.get<HitboxComponent>(m_player);

    // 检查碰撞盒是否激活
    if (!hb.isActive)
    {
        thLogger::debug("玩家碰撞盒未激活");
        return;
    }

    m_playerPosition = tf.position + hb.offset;
    m_playerRadius = hb.radius;

    // 1.检测弹幕碰撞
    auto bulletView = registry.view<BulletComponent, SpriteComponent, BulletCollider>();

    bulletView.each([&](entt::entity bullet, BulletComponent& bulletComp, SpriteComponent& tf, BulletCollider& bc)
                    {
                        if (bulletComp.isPlayerBullet)
                        {
                            thLogger::debug("跳过玩家弹幕");
                            return; // 忽略玩家弹幕
                        }

                        // 弹幕使用点碰撞
                        if (checkCollision(m_playerPosition, m_playerRadius, tf.position, bc.radius))
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

bool checkCollision(const glm::vec2& posA, float radiusA, const glm::vec2& posB, float radiusB)
{
    // 正交圆判定 当距离的平方小于二者半价平方之和的时候判定 即两条半径 和圆心连线构成直角三角形的时候判定
    const float dx = posB.x - posA.x;
    const float dy = posB.y - posA.y;

    const float distanceSquared = dx * dx + dy * dy;
    const float radiiSquared = radiusA * radiusA + radiusB * radiusB;

    return distanceSquared < radiiSquared;
}
} // namespace th::CollisionSystem
