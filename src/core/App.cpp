#include <core/App.hpp>
#include <core/Context.hpp>
#include <core/Window.hpp>
#include <utils/Logger.hpp>
#include <utils/Time.hpp>
#include "entt/entt.hpp"
#include "entity/component/PlayerComponent.hpp"
#include "entity/component/SpriteComponent.hpp"
#include "graphics/RenderSystem.hpp"
#include "core/InputSystem.hpp"
#include "entity/BulletSystem.hpp"
#include "entity/CollisionSystem.hpp"
#include "resources/AudioManager.hpp"
#include <GLFW/glfw3.h>

namespace th
{

    App::App()
    {
        init();
    }

    void App::run()
    {
        mainLoop();
    }

    void App::mainLoop()
    {
        auto &ctx = Context::getInstance();
        auto &window = Window::getInstance();
        auto &registry = ctx.getRegistry();
        auto &inputSystem = InputSystem::getInstance();
        auto &renderSystem = RenderSystem::getInstance();
        auto &ps = PlayerSystem::getInstance();
        auto &bs = BulletSystem::getInstance();
        auto &cs = CollisionSystem::getInstance();
        auto &am = AudioManager::getInstance();

        double lastFrameTime = glfwGetTime();

        while (!window.shouldClose())
        {
            Time::update();
            window.pollEvents();

            // 更新FPS
            update(lastFrameTime);
            // 处理输入
            inputSystem.processInput(registry);
            // 更新玩家移动
            ps.updateMove(registry);
            // 更新弹幕移动
            bs.update(registry, Time::getDeltaTime());
            // 处理碰撞
            cs.update(registry);

            renderSystem.update(registry);


            window.swapBuffers();
        }
    }

    void App::init()
    {
        Time::update();
        auto &ctx = Context::getInstance(); // 此时创建单例并初始化
        ctx.init();
    }

    void App::update(double lastPrintTime)
    {
        auto &window = Window::getInstance();
        // 输出FPS
        int fps = window.getFPS();
        double currentTime = glfwGetTime();
        if (currentTime - lastPrintTime >= 1.0)
        {
            thLogger::Log(thLogger::Info, "FPS:" + std::to_string(fps));
            lastPrintTime = currentTime;
        }
    }
} // namespace th
