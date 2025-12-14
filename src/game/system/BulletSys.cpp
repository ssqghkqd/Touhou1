module;
#include <glm/ext.hpp>
#include <entt/entt.hpp>
module game.system.BulletSys;
import core.ConfigManager;
import game.comp;


namespace th::BulletSys
{

entt::entity createBullet(entt::registry& registry,
                          const glm::vec2& position,
                          const glm::vec2& velocity,
                          const std::string& configName,
                          bool isPlayerBullet)
{
    static auto& cm = registry.ctx().get<ConfigManager>();
    static auto bDefault = cm.getBulletConfig(configName);
    const auto bullet = registry.create();

    // 变换组件
    auto& tf = registry.emplace<TransformComp>(bullet);
    tf.position = position;

    // 精灵组件
    auto& sc = registry.emplace<SpriteComp>(bullet);
    sc.velocity = velocity;
    // 弹幕组件
    auto& bulletComp = registry.emplace<BulletComp>(bullet);
    bulletComp.isPlayer = isPlayerBullet;

    // 渲染组件
    auto& render = registry.emplace<RenderComp>(bullet);
    render.textureName = bDefault.textureName;
    render.size = bDefault.size;

    // 碰撞组件
    auto& cc = registry.emplace<CollisionComp>(bullet);
    cc.radius = bDefault.radius;

    // 标签
    registry.emplace<BulletTag>(bullet);

    return bullet;
}
} // namespace th::BulletSystem