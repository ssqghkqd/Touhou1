#include "ecs/system/PlayerSystem.hpp"

#include <entt/entt.hpp>
#include <glm.hpp>

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "core/App.hpp"
#include "core/Window.hpp"
#include "ecs/comp/CollisionComp.hpp"
#include "ecs/comp/DragComp.hpp"
#include "ecs/comp/PlayerComp.hpp"
#include "ecs/comp/RenderComp.hpp"
#include "ecs/comp/SpriteComp.hpp"
#include "ecs/comp/TagComp.hpp"
#include "ecs/comp/TransformComp.hpp"

namespace th::PlayerSystem
{

void update(entt::registry& registry, float dt)
{
    constexpr int width = App::bgoffsetX + App::bgwidth;
    constexpr int height = App::bgoffsetY + App::bgheight;
    constexpr float slowMoveSpeed = 300.02f;
    constexpr float moveSpeed = 548.57f;

    static auto view = registry.view<PlayerComp, TransformComp, SpriteComp, RenderComp, DragComp>();

    view.each([&](entt::entity entity, auto& player, auto& tf, auto& sprite, auto& render, auto& drag)
              {
                  (void)entity;
                  // 1. 计算目标速度
                  float targetSpeed = player.isSlow ? slowMoveSpeed : moveSpeed;
                  glm::vec2 targetVelocity = player.targetDir * targetSpeed;

                  // 使用线性插值平滑过渡速度
                  sprite.velocity = glm::mix(sprite.velocity, targetVelocity, drag.smoothFactor);

                  // 应用速度到位置
                  tf.position += sprite.velocity * dt;

                  // 边界检查（考虑精灵尺寸）

                  const float halfWidth = render.size.x / 2.0f;
                  const float halfHeight = render.size.y / 2.0f;

                  tf.position.x = glm::clamp(tf.position.x, App::bgoffsetX + halfWidth, (float)width - halfWidth);
                  tf.position.y = glm::clamp(tf.position.y, App::bgoffsetY + halfHeight, (float)height - halfHeight);

                  // 记录位置变化
                  // thLogger::debug("玩家位置: (" + std::to_string(tf.position.x) + ", " + std::to_string(tf.position.y) + ")");
              });
}

entt::entity createPlayer(entt::registry& registry)
{
    const auto player = registry.create();

    auto& tf = registry.emplace<TransformComp>(player);
    // 精灵组件
    auto& sprite = registry.emplace<SpriteComp>(player);

    // 玩家控制组件
    auto& playerComp = registry.emplace<PlayerComp>(player);

    auto& render = registry.emplace<RenderComp>(player);
    auto& drag = registry.emplace<DragComp>(player);
    auto& cs = registry.emplace<CollisionComp>(player);

    // 玩家标签
    registry.emplace<PlayerTag>(player);

    tf.position = {App::bgwidth / 2.0f, App::bgheight / 2.0f}; // 屏幕中心
    render.size = {48.0f, 48.0f};                              // 适当尺寸
    render.textureName = "player";
    cs.radius = 2.0f;

    m_player = player;
    return player;
}

void shot(entt::registry& registry)
{
    auto& tf = registry.get<TransformComp>(m_player);
    //BulletSystem::createBullet(registry, tf.position, {0.0f, -500.0f}, true);
}

entt::entity& getPlayer()
{
    return m_player;
}

void updatePlayerMovement(entt::registry& registry)
{

    auto& tf = registry.get<TransformComp>(m_player);
    auto& pc = registry.get<PlayerComp>(m_player);
    // 检测低速模式
    static const auto& window = Window::getInstance();
    pc.isSlow = window.isKeyPressed(GLFW_KEY_LEFT_SHIFT);

    pc.targetDir = glm::vec2(0.0f); // 重置为零向量

    // 移动方向
    if (window.isKeyPressed(GLFW_KEY_UP))
        pc.targetDir.y = -1.0f;
    if (window.isKeyPressed(GLFW_KEY_DOWN))
        pc.targetDir.y = 1.0f;
    if (window.isKeyPressed(GLFW_KEY_LEFT))
        pc.targetDir.x = -1.0f;
    if (window.isKeyPressed(GLFW_KEY_RIGHT))
        pc.targetDir.x = 1.0f;

    // 标准化方向
    if (glm::length(pc.targetDir) > 0.0f)
    {
        pc.targetDir = glm::normalize(pc.targetDir);
    }
}

} // namespace th::PlayerSystem
