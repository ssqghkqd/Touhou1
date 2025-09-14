#include "ecs/EntityManager.hpp"

#include "ecs/BulletSystem.hpp"
#include "ecs/PlayerSystem.hpp"
#include "utils/Logger.hpp"

namespace th
{
EntityManager& EntityManager::getInstance()
{
    static EntityManager instance;
    return instance;
}

void EntityManager::init(entt::registry& registry)
{
    if (inited)
    {
        thLogger::warning("entitymanager已经初始化");
        return;
    }

    // 初始化过程

    PlayerSystem::getInstance().init();
    PlayerSystem::getInstance().createPlayer(registry);

    thLogger::info("玩家已创建");
    BulletSystem::getInstance().init(registry);

    thLogger::info("entitymanager初始化完成");
    inited = true;
}
} // namespace th
