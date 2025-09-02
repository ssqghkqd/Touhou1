#pragma once

#include "entt/entt.hpp"
#include "entity/PlayerSystem.hpp"
#include "core/Window.hpp"
#include "core/App.hpp"
#include "core/InputManager.hpp"

namespace th
{
    class InputSystem
    {
    public:
        void processInput(entt::registry &registry);
        static InputSystem &getInstance();
        void checkExit();
        void updatePlayerMovement(entt::registry &registry);
        void toggleDebug();
        void test();
        void toggleHitbox(entt::registry &registry);

        // 删除拷贝构造和赋值操作符
        InputSystem(const InputSystem &) = delete;
        InputSystem &operator=(const InputSystem &) = delete;

        // 移动语义也删除（单例不需要）
        InputSystem(InputSystem &&) = delete;
        InputSystem &operator=(InputSystem &&) = delete;

    private:
        InputSystem() = default;
        ~InputSystem() = default;

        int width = App::width;
        int height = App::height;

        Window &window = Window::getInstance();
        PlayerSystem &ps = PlayerSystem::getInstance();
        InputManager &im = InputManager::getInstance();
    };
}