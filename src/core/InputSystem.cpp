#include "core/InputSystem.hpp"

#include "core/App.hpp"
#include "core/Window.hpp"
#include "ecs/PlayerSystem.hpp"
#include "ecs/component/PlayerComponent.hpp"
#include "ecs/component/SpriteComponent.hpp"
#include "resources/AudioManager.hpp"
#include "utils/Logger.hpp"

namespace th
{
    InputSystem &InputSystem::getInstance()
    {
        static InputSystem instance;
        return instance;
    }

    void InputSystem::processInput(entt::registry &registry)
    {
        // 检查是否退出
        checkExit();
        toggleDebug();
        updatePlayerMovement(registry);
        toggleHitbox(registry);
        im.update();
    }

    void InputSystem::checkExit()
    {
        if (window.isKeyPressed(GLFW_KEY_ESCAPE))
        {
            window.close();
        }
    }

    void InputSystem::updatePlayerMovement(entt::registry &registry)
    {
        static auto view = registry.view<PlayerControl, TransformComponent>();
        view.each([&](entt::entity entity, PlayerControl &control, TransformComponent &tf)
                  {
                    
            // 避免未使用变量警告
            (void)entity;
            (void)tf;
            // 检测低速模式
            control.slowMode = window.isKeyPressed(GLFW_KEY_LEFT_SHIFT);

            control.targetDir = glm::vec2(0.0f);  // 重置为零向量

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
            } });
    }
    void InputSystem::toggleDebug()
    {
        if (im.isKeyJustPressed(GLFW_KEY_GRAVE_ACCENT))
        {
            thLogger::toggleDebug();
        }
    }

    void InputSystem::toggleHitbox(entt::registry &registry)
    {
        if (im.isKeyJustPressed(GLFW_KEY_P))
        {
            ps.toggleNoHitbox(registry);
            thLogger::info("游戏已暂停/恢复(切换玩家碰撞)");
        }
    }
} // namespace th
