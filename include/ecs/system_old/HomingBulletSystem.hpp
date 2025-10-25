#pragma once

#include <entt/entt.hpp>

namespace th::HomingBulletSystem
{

void init(entt::registry& registry);

void createHoming(entt::registry& registry, int way = 1, float angle = 0.0f);

inline bool inited = false;
inline entt::entity m_player;
inline glm::vec2 m_playerPosition{};
} // namespace th::HomingBulletSystem
