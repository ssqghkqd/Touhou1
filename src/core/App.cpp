module;

module core.App;
import utils.Time;
import spdlog;
import Config;
import core.Init;
import core.Window;
import core.InputSystem;
import graphics.RenderSystem;
import game.cmd.CmdPlayer;
import game.system.PlayerSys;
import game.system.BulletSys;
import game.system.CollisionSys;
import game.system.SpriteMovementSys;

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
    double lastStatTime = 0.0;
    while (!window.shouldClose())
    {
        Time::update();
        const double currentTime = Time::getTime();
        window.updateFPS();
        window.pollEvents();
        // 处理输入
        inputSystem.processInput(registry);
        // 更新玩家移动
        PlayerSys::update(registry, Time::getDeltaTime());
        // 更新所有精灵移动
        SpriteMovementSys::update(registry, Time::getDeltaTime());
        // 生成弹幕
        BulletSys::update(registry, Time::getDeltaTime(), Time::getTime());
        cmdp.update(registry);
        // 处理碰撞
        CollisionSys::update(registry);

        renderSystem.update(registry);

        if (currentTime - lastStatTime >= stat_interval) {
            update(audio);
            lastStatTime = currentTime; // 重置计时器
        }

        window.swapBuffers();
    }
}

void App::update(AudioManager& audioManager)
{
    audioManager.cleanSound();

}
} // namespace th
