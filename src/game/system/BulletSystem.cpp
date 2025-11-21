#include "game/system/BulletSystem.hpp"

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "core/ConfigManager.hpp"
#include "game/comp/BulletComp.hpp"
#include "game/comp/CollisionComp.hpp"
#include "game/comp/RenderComp.hpp"
#include "game/comp/SpriteComp.hpp"
#include "game/comp/TagComp.hpp"
#include "game/comp/TransformComp.hpp"
#include "spdlog/spdlog.h"

namespace th::BulletSystem
{

void update(entt::registry& registry, float dt, float t)
{
}

entt::entity createBullet(entt::registry& registry,
                          const glm::vec2& position,
                          const glm::vec2& velocity,
                          const std::string& texture_name,
                          bool isPlayerBullet,
                          bool isExistForever)
{
    static auto& cm = registry.ctx().get<ConfigManager>();
    static auto bDefault = cm.getBulletConfig("bullet_default");
    auto bullet = registry.create();

    // 变换组件
    auto& tf = registry.emplace<TransformComp>(bullet);
    tf.position = position;

    // 精灵组件
    auto& sc = registry.emplace<SpriteComp>(bullet);
    sc.velocity = velocity;
    // 弹幕组件
    auto& bulletComp = registry.emplace<BulletComp>(bullet);
    bulletComp.isPlayer = isPlayerBullet;
    bulletComp.isExistForever = isExistForever;

    // 渲染组件
    auto& render = registry.emplace<RenderComp>(bullet);
    render.textureName = texture_name;
    render.size = bDefault.size;

    // 碰撞组件
    auto& cc = registry.emplace<CollisionComp>(bullet);
    cc.radius = bDefault.radius;

    // 标签
    registry.emplace<BulletTag>(bullet);

    return bullet;
}
} // namespace th::BulletSystem