#include "core/InputSystem.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include "core/Window.hpp"
#include "resources/AudioManager.hpp"
#include "utils/Logger.hpp"
#include "utils/Time.hpp"
#include "ecs/system/PlayerSystem.hpp"
#include <ext.hpp>

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
    PlayerSystem::updatePlayerMovement(registry);
    //toggleHitbox(registry);
    shot(registry);
    im.update();
}

void InputSystem::checkExit() const
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
void InputSystem::toggleDebug()
{
    if (im.isKeyJustPressed(GLFW_KEY_GRAVE_ACCENT))
    {
        thLogger::toggleDebug();
    }
}

} // namespace th
