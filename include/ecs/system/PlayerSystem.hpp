#pragma once

#include "../../../thirdparty/entt/src/entt/entt.hpp"

namespace th::PlayerSystem
{
void init();
void updateMove(entt::registry& registry);
void toggleNoHitbox(entt::registry& registry);
entt::entity createPlayer(entt::registry& registry);
entt::entity& getPlayer();

inline bool inited = false;
inline entt::entity m_player;

} // namespace th::PlayerSystem
