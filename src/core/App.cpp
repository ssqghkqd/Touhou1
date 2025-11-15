#include <core/App.hpp>
#include <core/Window.hpp>
#include <utils/Time.hpp>

#include "core/Init.hpp"
#include "core/InputSystem.hpp"
#include "game/cmd/CmdPlayer.hpp"
#include "game/system/BulletSystem.hpp"
#include "game/system/CollisionSystem.hpp"
#include "game/system/PlayerSystem.hpp"
#include "game/system/SpriteMovementSys.hpp"
#include "graphics/RenderSystem.hpp"
#include "resources/AudioManager.hpp"
#include "spdlog/spdlog.h"

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
    auto& audio = registry.ctx().get<AudioManager>();
    auto& cmdp = registry.ctx().get<cmd::CmdPlayer>();

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
        BulletSystem::update(registry, Time::getDeltaTime(), Time::getWindowTime());
        cmdp.update(registry);
        // 处理碰撞
        CollisionSystem::update(registry);

        renderSystem.update(registry);

        audio.cleanSound();

        window.swapBuffers();
    }
}
} // namespace th
