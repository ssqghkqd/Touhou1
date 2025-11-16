#pragma once
#include <entt/entt.hpp>

namespace th::Init
{
void init(entt::registry& reg);

void loadResources(entt::registry& reg);

void loadCore(entt::registry& reg);
void gameStatusSet(entt::registry& reg);
} // namespace th::Init