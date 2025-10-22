#include "ecs/system/EnemySystem.hpp"

#include "ecs/component/EnemyComponent.hpp"
#include "ecs/component/SpriteComponent.hpp"
#include "entt/entity/registry.hpp"

namespace th::EnemySystem
{

void spawnEnemy(entt::registry& registry, const glm::vec2& position)
{
    auto enemy = registry.create();

    auto& sc = registry.emplace<SpriteComponent>(enemy);

}

}