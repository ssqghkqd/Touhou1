module;
#include <entt/entt.hpp>

module core.InputSystem;
import core.Window;
import glfw;
import game.system.PlayerSys;

namespace th
{

void InputSystem::processInput(entt::registry& registry)
{
    // 检查是否退出
    checkExit(registry);
    PlayerSys::updatePlayerMovement(registry);
    shot(registry);
    update(registry);
}

void InputSystem::checkExit(entt::registry& reg) const
{
    auto& window = reg.ctx().get<Window>();
    if (window.isKeyPressed(glfw::KEY_ESCAPE))
    {
        window.close();
    }
}

void InputSystem::shot(entt::registry& registry)
{
    auto& window = registry.ctx().get<Window>();
    if (window.isKeyPressed(glfw::KEY_Z))
    {
        PlayerSys::shot(registry);
    }
}



void InputSystem::update(entt::registry& reg)
{
    auto& window = reg.ctx().get<Window>();
    // 更新所有按键状态
    for (auto& [key, state] : keyStates)
    {
        state.previous = state.current;
        state.current = window.isKeyPressed(key);
    }
}

bool InputSystem::isKeyJustPressed(int key)
{
    // 确保按键已注册
    if (keyStates.find(key) == keyStates.end())
    {
        keyStates[key] = {false, false};
    }

    auto& state = keyStates[key];
    return state.current && !state.previous;
}

bool InputSystem::isKeyDown(int key) const
{
    auto it = keyStates.find(key);
    return it != keyStates.end() && it->second.current;
}

bool InputSystem::isKeyReleased(int key) const
{
    auto it = keyStates.find(key);
    return it != keyStates.end() && !it->second.current;
}

} // namespace th
