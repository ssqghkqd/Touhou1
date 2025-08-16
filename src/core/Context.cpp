#include <core/Context.hpp>
#include <core/App.hpp>
#include <utils/Logger.hpp>
#include <stdafx.hpp>
#include "core/Window.hpp"
#include "core/InputSystem.hpp"
#include "graphics/MeshManager.hpp"
#include "graphics/RenderSystem.hpp"
#include "graphics/ShaderManager.hpp"
#include "graphics/TextureManager.hpp"
#include "entity/EntityManager.hpp"
#include "entity/CollisionSystem.hpp"
#include "resources/AudioManager.hpp"

namespace th
{
    Context &Context::getInstance()
    {
        static Context instance;
        return instance;
    }

    Context::~Context()
    {
        glfwTerminate();
    }

    entt::registry &Context::getRegistry()
    {
        return registry;
    }

    // 初始化所有资源
    void Context::init()
    {
        if (contextInitialized)
        {
            contextInitialized = true;
            thLogger::Log(thLogger::Warning, "Context已经初始化");
            return;
        }

        Window::getInstance();
        InputSystem::getInstance();
        MeshManager::getInstance();
        RenderSystem::getInstance();
        ShaderManager::getInstance();
        TextureManager::getInstance();
        EntityManager::getInstance().init(registry);

        auto &audio = AudioManager::getInstance();
        audio.init();

        // BulletSystem::getInstance().createStaticBullet(registry);
        CollisionSystem::getInstance().init(registry);
        audio.loadSound("miss", "miss.ogg");
        audio.setMasterVolume(30.0f); 

        contextInitialized = true;
    }

    
}