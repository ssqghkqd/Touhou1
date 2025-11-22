//
// Created by ss on 2025/11/22.
//

module;
#include <entt/entt.hpp>
export module core.Init;

export namespace th::Init
{
void init(entt::registry& reg);

void loadResources(entt::registry& reg);

void loadCore(entt::registry& reg);
void gameStatusSet(entt::registry& reg);
} // namespace th::Init