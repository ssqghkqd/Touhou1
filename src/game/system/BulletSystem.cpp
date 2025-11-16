#include "game/system/BulletSystem.hpp"

#include <cmath>
#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "core/App.hpp"
#include "game/comp/BulletComp.hpp"
#include "game/comp/CollisionComp.hpp"
#include "game/comp/RenderComp.hpp"
#include "game/comp/SpriteComp.hpp"
#include "game/comp/TagComp.hpp"
#include "game/comp/TransformComp.hpp"
#include "json.hpp"
#include "utils/JsonManager.hpp"

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
                          bool isExistForever,
                          bool isSeg)
{
    static nlohmann::json& bulletJ = JsonManager::get("config.bullet");
    static nlohmann::json& bDefault = bulletJ["bullet_default"];
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
    bulletComp.isSeg = isSeg;

    // 渲染组件
    auto& render = registry.emplace<RenderComp>(bullet);
    render.textureName = texture_name;
    render.size = {bDefault["width"], bDefault["height"]};

    // 碰撞组件
    auto& cc = registry.emplace<CollisionComp>(bullet);
    cc.radius = bDefault["collision_radius"];

    // 标签
    registry.emplace<BulletTag>(bullet);

    return bullet;
}
} // namespace th::BulletSystem