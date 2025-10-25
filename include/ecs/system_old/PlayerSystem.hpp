#pragma once

#include "../../../thirdparty/entt/src/entt/entt.hpp"

namespace th::PlayerSystem
{
void init();
void updateMove(entt::registry& registry);
void toggleNoHitbox(entt::registry& registry);
entt::entity createPlayer(entt::registry& registry);
entt::entity& getPlayer();
void shot(entt::registry& registry);

inline bool inited = false;
inline entt::entity m_player;
inline float shotInterval = 0.5f;

} // namespace th::PlayerSystem
