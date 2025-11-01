#pragma once
#include <entt/entt.hpp>

namespace th::Init
{
void init(entt::registry& reg);

void loadResources(entt::registry& reg);

void setStatus(entt::registry& reg);
void gameSetup(entt::registry& reg);
} // namespace th::Init