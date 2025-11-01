#include "ecs/system/BulletSystem.hpp"

#include <entt/entt.hpp>
#include <glm/glm.hpp>

#include "core/App.hpp"
#include "ecs/comp/BulletComp.hpp"
#include "ecs/comp/CollisionComp.hpp"
#include "ecs/comp/RenderComp.hpp"
#include "ecs/comp/SpriteComp.hpp"
#include "ecs/comp/TagComp.hpp"
#include "ecs/comp/TransformComp.hpp"
#include "json.hpp"
#include "utils/JsonManager.hpp"
#include "utils/Random.hpp"

namespace th::BulletSystem
{

float spawnTimer = 0.0f;
float spawnInterval = 1.0f;

void update(entt::registry& registry, float dt)
{
    // 这里是测试的随机弹幕
    // 更新计时器
    spawnTimer += dt;

    // 检查是否应该生成新弹幕
    if (spawnTimer >= spawnInterval)
    {
        spawnTimer = 0.0f;
        for (int i = 0; i < 10; i++)
        {
            const float x = Random::range(App::bgoffsetX + 10.0f,
                                          App::bgoffsetX + App::bgwidth - 10.0f);
            const float y = Random::range(App::bgoffsetY + 50.0f,
                                          App::bgoffsetY + 80.0f);
            const float vx = Random::range(-50.0f, 50.0f);
            const float vy = Random::range(200.0f, 300.0f);
            createBullet(registry, {x, y}, {vx, vy});
        }
    }

    // 这里是测试的随机弹幕
}

entt::entity createBullet(entt::registry& registry, const glm::vec2& position, const glm::vec2& velocity, bool isPlayerBullet, bool isExistForever)
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

    // 渲染组件
    auto& render = registry.emplace<RenderComp>(bullet);
    render.textureName = bDefault["texture_name"];
    render.size = {bDefault["width"], bDefault["height"]};
    // if (isPlayerBullet)
    // {
    //     render.color = {1.0f, 0.0f, 0.0f, 1.0f};
    // }

    // 碰撞组件
    auto& cc = registry.emplace<CollisionComp>(bullet);
    cc.radius = bDefault["collision_radius"];

    // 标签
    registry.emplace<BulletTag>(bullet);

    return bullet;
}
} // namespace th::BulletSystem