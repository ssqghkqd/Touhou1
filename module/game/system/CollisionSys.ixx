//
// Created by ss on 2025/11/22.
//
module;
#include <glm/ext.hpp>
#include <entt/entt.hpp>
export module game.system.CollisionSys;

export namespace th::CollisionSys
{
void update(entt::registry& registry);
bool checkCollision(const glm::vec2& posA, const float radiusA, const glm::vec2& posB, const float radiusB);
}
