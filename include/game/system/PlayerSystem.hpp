#pragma once
#include "entt/entity/registry.hpp"

namespace th::PlayerSystem
{
void update(entt::registry& registry, float dt);
entt::entity createPlayer(entt::registry& registry);
void shot(entt::registry& registry);
entt::entity& getPlayer();
void updatePlayerMovement(entt::registry& registry);

inline entt::entity m_player;
inline float shotInterval = 0.1f;
inline float m_lastTime = 0.0f;
} // namespace PlayerSystem