#include <GLFW/glfw3.h>

#include <core/App.hpp>
#include <core/Window.hpp>
#include <utils/Logger.hpp>
#include <utils/Time.hpp>

#include "core/InputSystem.hpp"
#include "../../include/ecs/system/BulletSystem.hpp"
#include "ecs/system/CollisionSystem.hpp"
#include "graphics/RenderSystem.hpp"
#include "graphics/ShaderManager.hpp"
#include "graphics/TextureManager.hpp"
#include "resources/AudioManager.hpp"

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
        auto &window = Window::getInstance();
        auto &inputSystem = InputSystem::getInstance();
        auto &renderSystem = RenderSystem::getInstance();
        // auto &am = AudioManager::getInstance();

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
            PlayerSystem::updateMove(registry);
            // 更新弹幕移动
            BulletSystem::update(registry, Time::getDeltaTime());
            // 处理碰撞
            CollisionSystem::update(registry);

            renderSystem.update(registry);


            window.swapBuffers();
        }
    }

    void App::init()
    {
        Time::update();
        Window::getInstance();
        InputSystem::getInstance();
        MeshManager::getInstance();
        RenderSystem::getInstance();
        ShaderManager::getInstance();
        TextureManager::getInstance();
        PlayerSystem::init();
        PlayerSystem::createPlayer(registry);

        thLogger::info("玩家已创建");
        BulletSystem::init(registry);
        CollisionSystem::init(registry);

        auto &audio = AudioManager::getInstance();
        audio.init();
        audio.loadSound("miss", "miss.wav");
        audio.setMasterVolume(1.0f);
        audio.playMusic("th11_09.wav");
    }

    void App::update(double& lastPrintTime)
    {
        auto &window = Window::getInstance();
        // 输出FPS
        const int fps = window.getFPS();
        const double currentTime = glfwGetTime();
        if (currentTime - lastPrintTime >= 1.0)
        {
            thLogger::Log(thLogger::Info, "FPS:" + std::to_string(fps));
            lastPrintTime = currentTime;
        }
    }
} // namespace th
