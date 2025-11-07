#include "ecs/system/BulletSystem.hpp"

#include <cmath>
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
#include "spdlog/spdlog.h"
#include "utils/JsonManager.hpp"

namespace th::BulletSystem
{

float spawnTimer = 0.0f;
float spawnInterval = 0.05f;
static double base_angle = 0.0f;
static double angle_velocity = 0.0f;

void update(entt::registry& registry, float dt, float t)
{
    // 这里是测试的随机弹幕
    spawnTimer += dt;
    angle_velocity = 5 * std::sin(t / 2);
    base_angle += angle_velocity * dt;
    base_angle = fmod(base_angle, 2.0 * M_PI);

    if (spawnTimer >= spawnInterval)
    {
        spawnTimer = 0.0f;
        for (int i = 0; i < 12; i++)
        {
            const double angle = base_angle + i * (M_PI / 6);
            auto v = glm::vec2(std::cos(angle), std::sin(angle)) * 450.0f;
            constexpr float x = App::bgoffsetX + App::bgwidth * 0.5f;
            constexpr float y = App::bgoffsetY + App::bgheight * 0.3f;
            createBullet(registry, {x, y}, v, "xiaoyu");

        }
    }

    // 这里是测试的随机弹幕
}

entt::entity createBullet(entt::registry& registry,
                          const glm::vec2& position,
                          const glm::vec2& velocity,
                          const std::string& texture_name,
                          bool isPlayerBullet,
                          bool isExistForever)
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