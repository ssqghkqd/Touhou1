#pragma once
#include <entt/entt.hpp>
#include <glm.hpp>
namespace th::EnemySys
{
void update(entt::registry& reg, float dt);
entt::entity spawnEnemy(entt::registry& reg, glm::vec2 pos);
void collision(entt::registry& reg);
}