#include "ecs/system/BulletSystem.hpp"

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "ecs/comp/BulletComp.hpp"
#include "ecs/comp/CollisionComp.hpp"
#include "ecs/comp/RenderComp.hpp"
#include "ecs/comp/SpriteComp.hpp"
#include "ecs/comp/TagComp.hpp"
#include "ecs/comp/TransformComp.hpp"

namespace th::BulletSystem
{

float spawnTimer = 0.0f;
float spawnInterval = 0.5f;
void update(entt::registry& registry, float dt)
{
    // 这里是测试的随机弹幕
    // 更新计时器
    spawnTimer += dt;

    // 检查是否应该生成新弹幕
    if (spawnTimer >= spawnInterval)
    {
        spawnTimer = 0.0f;
        createBullet(registry, {600.0f, 100.0f}, {0.0f, 300.0f});
    }

    // 这里是测试的随机弹幕
}

entt::entity createBullet(entt::registry& registry, const glm::vec2& position, const glm::vec2& velocity, bool isPlayerBullet, bool isExistForever)
{
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
    render.textureName = "xiaoyu";
    render.size = {64.0f, 64.0f};

    // 碰撞组件
    registry.emplace<CollisionComp>(bullet);

    // 标签
    registry.emplace<BulletTag>(bullet);

    return bullet;
}
}