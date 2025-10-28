#include <core/App.hpp>
#include <core/Window.hpp>
#include <utils/Time.hpp>

#include "core/Init.hpp"
#include "core/InputSystem.hpp"
#include "ecs/system/BulletSystem.hpp"
#include "ecs/system/CollisionSystem.hpp"
#include "ecs/system/PlayerSystem.hpp"
#include "ecs/system/SpriteMovementSys.hpp"
#include "graphics/RenderSystem.hpp"
#include "graphics/ShaderManager.hpp"
#include "graphics/TextureManager.hpp"
#include "resources/AudioManager.hpp"

namespace th
{

App::App()
{
    Init::init(registry);
}

void App::run()
{
    mainLoop();
}

void App::mainLoop()
{
    auto& window = registry.ctx().get<Window>();
    auto& inputSystem = registry.ctx().get<InputSystem>();
    auto& renderSystem = registry.ctx().get<RenderSystem>();

    while (!window.shouldClose())
    {
        Time::update();
        window.pollEvents();

        // 更新FPS
        window.updateFPS();
        // 处理输入
        inputSystem.processInput(registry);
        // 更新玩家移动
        PlayerSystem::update(registry, Time::getDeltaTime());
        // 更新所有精灵移动
        SpriteMovementSys::update(registry, Time::getDeltaTime());
        // 生成弹幕
        BulletSystem::update(registry, Time::getDeltaTime());
        // 处理碰撞
        CollisionSystem::update(registry);

        renderSystem.update(registry);

        window.swapBuffers();
    }
}
} // namespace th
