#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <core/App.hpp>
#include <core/Window.hpp>
#include <iostream>
#include <utils/Logger.hpp>
#include <utils/Time.hpp>

#include "core/InputSystem.hpp"
#include "ecs/system/BulletSystem.hpp"
#include "ecs/system/CollisionSystem.hpp"
#include "ecs/system/PlayerSystem.hpp"
#include "ecs/system/SpriteMovementSys.hpp"
#include "graphics/RenderSystem.hpp"
#include "graphics/ShaderManager.hpp"
#include "graphics/TextureManager.hpp"
#include "resources/AudioManager.hpp"
#include "spdlog/spdlog.h"

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
            PlayerSystem::update(registry, Time::getDeltaTime());
            // 更新所有精灵移动
            SpriteMovementSys::update(registry, Time::getDeltaTime());
            // 生成弹幕
            BulletSystem::update(registry, Time::getDeltaTime());
            // 处理碰撞
            CollisionSystem::update(registry);

            renderSystem.update(registry);


            window.swapBuffers();

            GLenum err;
            while ((err = glGetError()) != GL_NO_ERROR) {
                std::cerr << "OpenGL 错误: " << err << std::endl;
            }
        }
    }

    void App::init()
    {
        spdlog::set_pattern("[%H:%M:%S] [%l] %g:%# - %v");
        Time::update();
        Window::getInstance();
        InputSystem::getInstance();
        MeshManager::getInstance();
        RenderSystem::getInstance();
        ShaderManager::getInstance();
        TextureManager::getInstance();
        PlayerSystem::createPlayer(registry);

        //BulletSystem::init(registry);
        auto &audio = AudioManager::getInstance();
        audio.init();
        audio.loadSound("miss", "miss.wav");
        audio.setMasterVolume(1.0f);
        //audio.playMusic("th11_09.wav");
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
