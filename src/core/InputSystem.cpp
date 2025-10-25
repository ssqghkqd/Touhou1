#include "core/InputSystem.hpp"

#include "../../include/ecs/system_old/PlayerSystem.hpp"
#include "core/Window.hpp"
#include "ecs/component_old/PlayerComponent.hpp"
#include "ecs/component_old/SpriteComponent.hpp"
#include "resources/AudioManager.hpp"
#include "utils/Logger.hpp"
#include "utils/Time.hpp"

namespace th
{
InputSystem& InputSystem::getInstance()
{
    static InputSystem instance;
    return instance;
}

void InputSystem::processInput(entt::registry& registry)
{
    // 检查是否退出
    checkExit();
    toggleDebug();
    updatePlayerMovement(registry);
    toggleHitbox(registry);
    shot(registry);
    im.update();
}

void InputSystem::checkExit()
{
    if (window.isKeyPressed(GLFW_KEY_ESCAPE))
    {
        window.close();
    }
}

void InputSystem::shot(entt::registry& registry)
{
    if (window.isKeyPressed(GLFW_KEY_Z))
    {
        const float currentTime = Time::getWindowTime();

        // 每1秒更新一次FPS
        if (currentTime - m_lastTime >= PlayerSystem::shotInterval)
        {
            m_lastTime = currentTime;
            PlayerSystem::shot(registry);
        }
    }
}
void InputSystem::updatePlayerMovement(entt::registry& registry)
{
    static auto view = registry.view<PlayerControl, SpriteComponent>();
    view.each([&](entt::entity entity, PlayerControl& control, SpriteComponent& tf)
              {
                  // 避免未使用变量警告
                  (void)entity;
                  (void)tf;
                  // 检测低速模式
                  control.slowMode = window.isKeyPressed(GLFW_KEY_LEFT_SHIFT);

                  control.targetDir = glm::vec2(0.0f); // 重置为零向量

                  // 移动方向
                  if (window.isKeyPressed(GLFW_KEY_UP))
                      control.targetDir.y = -1.0f;
                  if (window.isKeyPressed(GLFW_KEY_DOWN))
                      control.targetDir.y = 1.0f;
                  if (window.isKeyPressed(GLFW_KEY_LEFT))
                      control.targetDir.x = -1.0f;
                  if (window.isKeyPressed(GLFW_KEY_RIGHT))
                      control.targetDir.x = 1.0f;

                  // 标准化方向
                  if (glm::length(control.targetDir) > 0.0f)
                  {
                      control.targetDir = glm::normalize(control.targetDir);
                  }
              });
}
void InputSystem::toggleDebug()
{
    if (im.isKeyJustPressed(GLFW_KEY_GRAVE_ACCENT))
    {
        thLogger::toggleDebug();
    }
}

void InputSystem::toggleHitbox(entt::registry& registry)
{
    if (im.isKeyJustPressed(GLFW_KEY_P))
    {
        PlayerSystem::toggleNoHitbox(registry);
        thLogger::info("游戏已暂停/恢复(切换玩家碰撞)");
    }
}
} // namespace th
