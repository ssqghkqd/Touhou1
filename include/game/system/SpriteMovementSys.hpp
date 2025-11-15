#pragma once
#include "entt/entity/registry.hpp"
#include "vec2.hpp"

namespace th::SpriteMovementSys
{
void update(entt::registry& registry, float dt);
}