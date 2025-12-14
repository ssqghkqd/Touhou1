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

void InputSystem::checkExit(entt::registry& reg)
{
    const auto& window = reg.ctx().get<Window>();
    if (window.isKeyPressed(glfw::key_escape))
    {
        window.close();
    }
}

void InputSystem::shot(entt::registry& registry)
{
    const auto& window = registry.ctx().get<Window>();
    if (window.isKeyPressed(glfw::key_z))
    {
        PlayerSys::shot(registry);
    }
}



void InputSystem::update(entt::registry& reg)
{
    const auto& window = reg.ctx().get<Window>();
    // 更新所有按键状态
    for (auto& [key, state] : m_keyStates)
    {
        state.previous = state.current;
        state.current = window.isKeyPressed(key);
    }
}

bool InputSystem::isKeyJustPressed(int key)
{
    // 确保按键已注册
    if (!m_keyStates.contains(key))
    {
        m_keyStates[key] = {false, false};
    }

    const auto& state = m_keyStates[key];
    return state.current && !state.previous;
}

bool InputSystem::isKeyDown(int key) const
{
    const auto it = m_keyStates.find(key);
    return it != m_keyStates.end() && it->second.current;
}

bool InputSystem::isKeyReleased(int key) const
{
    const auto it = m_keyStates.find(key);
    return it != m_keyStates.end() && !it->second.current;
}

} // namespace th
