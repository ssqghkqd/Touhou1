#pragma once

#include "entt/entt.hpp"

namespace th::CollisionSystem
{
void init(entt::registry& registry);
void update(entt::registry& registry);

inline bool inited = false;
// 玩家实体（缓存提高性能）
inline entt::entity m_player = entt::null;

// 玩家碰撞数据（避免每帧重复获取）
inline glm::vec2 m_playerPosition = {0.0f, 0.0f};
inline float m_playerRadius = 0.0f;

// 碰撞检测方法
bool checkCollision(const glm::vec2& posA, float radiusA, const glm::vec2& posB, float radiusB);

} // namespace th::CollisionSystem
