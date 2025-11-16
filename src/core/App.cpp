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

    Time::gameStart();
    spdlog::info("游戏开始");
    double lastStatTime = 0.0f;
    while (!window.shouldClose())
    {
        Time::update();
        const double currentTime = Time::getTime();
        window.pollEvents();
        // 处理输入
        inputSystem.processInput(registry);
        // 更新玩家移动
        PlayerSystem::update(registry, Time::getDeltaTime());
        // 更新所有精灵移动
        SpriteMovementSys::update(registry, Time::getDeltaTime());
        // 生成弹幕
        BulletSystem::update(registry, Time::getDeltaTime(), Time::getTime());
        cmdp.update(registry);
        // 处理碰撞
        CollisionSystem::update(registry);

        renderSystem.update(registry);

        if (currentTime - lastStatTime >= STAT_INTERVAL) {
            update(currentTime, audio);
            lastStatTime = currentTime; // 重置计时器
        }

        window.swapBuffers();
    }
}

void App::update(double currentTime, AudioManager& audioManager)
{
    const double fps = 1.0 / Time::getDeltaTime(); // 计算实时FPS
    spdlog::info("fps: {}, t: {}, dt: {}", fps, currentTime, Time::getDeltaTime());

    audioManager.cleanSound();

}
} // namespace th
