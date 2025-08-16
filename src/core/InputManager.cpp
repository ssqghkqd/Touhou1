#include "core/InputManager.hpp"
#include "core/Window.hpp"

namespace th
{
    InputManager& InputManager::getInstance()
    {
        static InputManager instance;
        return instance;
    }
    
    void InputManager::update()
    {
        auto &window = Window::getInstance();
        // 更新所有按键状态
        for (auto& [key, state] : keyStates) {
            state.previous = state.current;
            state.current = window.isKeyPressed(key);
        }
    }
    
    bool InputManager::isKeyJustPressed(int key)
    {
        // 确保按键已注册
        if (keyStates.find(key) == keyStates.end()) {
            keyStates[key] = {false, false};
        }
        
        auto& state = keyStates[key];
        return state.current && !state.previous;
    }
    
    bool InputManager::isKeyDown(int key) const
    {
        auto it = keyStates.find(key);
        return it != keyStates.end() && it->second.current;
    }
    
    bool InputManager::isKeyReleased(int key) const
    {
        auto it = keyStates.find(key);
        return it != keyStates.end() && !it->second.current;
    }
}