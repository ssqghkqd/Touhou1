#pragma once
#include "entt/entity/registry.hpp"
#include "vec2.hpp"

namespace th::BulletSystem
{
void update(entt::registry& registry, float dt);
entt::entity createBullet(entt::registry& registry, const glm::vec2& position, const glm::vec2& velocity, bool isPlayerBullet = false, bool isExistForever = true);
}