module;
#include <glm/ext.hpp>
#include <entt/entt.hpp>
module game.system.CollisionSys;
import spdlog;
import game.system.PlayerSys;
import game.comp;
import resources.AudioManager;

namespace th::CollisionSys
{

void update(entt::registry& registry)
{
    const auto m_player = PlayerSys::getPlayer();

    // 获取玩家碰撞数据
    const auto& tf = registry.get<TransformComp>(m_player);
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
                            spdlog::debug("跳过玩家弹幕");
                            return; // 忽略玩家弹幕
                        }

                        // 弹幕使用点碰撞
                        if (checkCollision(m_playerPosition, m_playerRadius, transform.position, collision.radius))
                        {
                            spdlog::debug("玩家碰撞弹幕");
                            // 播放被弹音效
                            auto& audio = registry.ctx().get<AudioManager>();
                            audio.playSound("miss", 0.6f, m_playerPosition);
                            registry.destroy(bullet);
                        }
                    });
}

bool checkCollision(const glm::vec2& posA, const float radiusA, const glm::vec2& posB, const float radiusB)
{
    /*
     * 正交圆判定 与东方原作相同
     * 不是相切时判定 当圆心连线与两个半径形成直角三角形时判定 相比相切可容忍范围增大了sqrt(2)r
     * 直接按照数学上的判定会导致手感很差，这种比较合适，性能也很合适
     */
    const float dx = posB.x - posA.x;
    const float dy = posB.y - posA.y;

    const float distanceSquared = dx * dx + dy * dy;
    const float radiiSquared = radiusA * radiusA + radiusB * radiusB;

    return distanceSquared < radiiSquared;
}

} // namespace th::CollisionSystem