#include "ecs/system/PlayerSystem.hpp"

#include "core/App.hpp"
#include "ecs/component/PlayerComponent.hpp"
#include "ecs/component/RenderComponent.hpp"
#include "ecs/component/SpriteComponent.hpp"
#include "utils/Time.hpp"

namespace th::PlayerSystem
{

void init()
{
    if (inited)
    {
        thLogger::warning("playersystem已初始化");
        return;
    }
    inited = true;
}

void updateMove(entt::registry& registry)
{
    float deltaTime = Time::getDeltaTime();
    constexpr int width = App::bgoffsetX + App::bgwidth;
    constexpr int height = App::bgoffsetY + App::bgheight;

    static auto view = registry.view<PlayerControl, TransformComponent>();

    view.each([&](entt::entity entity, PlayerControl& control, TransformComponent& tf)
              {
                  (void)entity;
                  // 1. 计算目标速度
                  float targetSpeed = control.slowMode ? control.slowMoveSpeed : control.moveSpeed;
                  glm::vec2 targetVelocity = control.targetDir * targetSpeed;

                  // 使用线性插值平滑过渡速度
                  control.velocity = glm::mix(control.velocity, targetVelocity, control.smoothFactor);

                  // 应用速度到位置
                  tf.position += control.velocity * deltaTime;

                  // 边界检查（考虑精灵尺寸）
                  auto render = registry.try_get<RenderComponent>(entity);
                  if (render)
                  {
                      float halfWidth = render->size.x / 2.0f;
                      float halfHeight = render->size.y / 2.0f;

                      tf.position.x = glm::clamp(tf.position.x, App::bgoffsetX + halfWidth, (float)width - halfWidth);
                      tf.position.y = glm::clamp(tf.position.y, App::bgoffsetY + halfHeight, (float)height - halfHeight);
                  }
                  else
                  {
                      // 没有精灵尺寸时的备用边界检查
                      tf.position.x = glm::clamp(tf.position.x, 0.0f, (float)width);
                      tf.position.y = glm::clamp(tf.position.y, 0.0f, (float)height);
                  }

                  // 记录位置变化
                  // thLogger::debug("玩家位置: (" + std::to_string(tf.position.x) + ", " + std::to_string(tf.position.y) + ")");
              });
}

void toggleNoHitbox(entt::registry& registry)
{
    static auto view = registry.view<HitboxComponent>();

    view.each([&](entt::entity entity, HitboxComponent& hc)
              {
                  (void)entity;
                  hc.isActive = !hc.isActive;
                  thLogger::warning("目前是否启用碰撞:" + std::to_string(hc.isActive));
              });
}

entt::entity createPlayer(entt::registry& registry)
{
    const auto player = registry.create();

    // 精灵组件
    auto& sprite = registry.emplace<TransformComponent>(player);

    // 玩家控制组件
    auto& playerCtrl = registry.emplace<PlayerControl>(player);

    // 判定点组件
    auto& hitbox = registry.emplace<HitboxComponent>(player);
    auto& render = registry.emplace<RenderComponent>(player);

    // 玩家标签
    registry.emplace<PlayerTag>(player);

    sprite.position = {App::bgwidth / 2.0f, App::bgheight / 2.0f}; // 屏幕中心
    render.size = {48.0f, 48.0f};                                  // 适当尺寸
    render.meshType = MeshType::Rect;
    render.textureName = "player";
    hitbox.textureName = "hitbox";
    hitbox.renderRadius = 10.0f;

    m_player = player;
    return player;
}

entt::entity& getPlayer()
{
    return m_player;
}
} // namespace th::PlayerSystem
