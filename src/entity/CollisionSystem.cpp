#include "entity/CollisionSystem.hpp"
#include "entity/component/BulletComponent.hpp"
#include "entity/component/PlayerComponent.hpp"
#include "entity/component/SpriteComponent.hpp"
#include "resources/AudioManager.hpp"


namespace th
{
CollisionSystem& CollisionSystem::getInstance()
{
    static CollisionSystem instance;
    return instance;
}

void CollisionSystem::init(entt::registry& registry)
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
    thLogger::info("碰撞系统初始化完成");
    inited = true;
}

void CollisionSystem::update(entt::registry& registry)
{

    // 如果没有玩家，直接返回
    if (m_player == entt::null || !registry.valid(m_player))
    {
        thLogger::warning("玩家实体无效");
        return;
    }

    // 确保玩家有必要的组件
    if (!registry.all_of<TransformComponent, HitboxComponent>(m_player))
    {
        thLogger::error("玩家缺少必要组件");
        return;
    }

    // 获取玩家碰撞数据
    auto& tf = registry.get<TransformComponent>(m_player);
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
    auto bulletView = registry.view<BulletComponent, TransformComponent, BulletCollider>();

    bulletView.each([&](entt::entity bullet, BulletComponent& bulletComp, TransformComponent& tf, BulletCollider& bc) {
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
            audio.playSound("miss", 60.0f, m_playerPosition);
            registry.destroy(bullet);
        }
    });
}

bool CollisionSystem::checkCollision(const glm::vec2& posA, float radiusA, const glm::vec2& posB, float radiusB)
{
    float distance = glm::distance(posA, posB);
    return distance < (radiusA + radiusB) * 0.5f;
}
} // namespace th
