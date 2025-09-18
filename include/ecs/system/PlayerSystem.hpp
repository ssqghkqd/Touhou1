#pragma once

#include "../../../thirdparty/entt/src/entt/entt.hpp"

namespace th::PlayerSystem
{
void init();
void updateMove(entt::registry& registry);
void toggleNoHitbox(entt::registry& registry);
entt::entity createPlayer(entt::registry& registry);

inline bool inited = false;

} // namespace th::PlayerSystem
