#pragma once
#include "entt/entity/registry.hpp"
#include "vec2.hpp"

namespace th::CollisionSystem
{
void update(entt::registry& registry);
bool checkCollision(const glm::vec2& posA, const float radiusA, const glm::vec2& posB, const float radiusB);
}