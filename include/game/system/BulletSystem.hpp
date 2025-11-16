#pragma once
#include "entt/entity/registry.hpp"
#include "vec2.hpp"

namespace th::BulletSystem
{
void update(entt::registry& registry, float dt, float t);
entt::entity createBullet(entt::registry& registry,
                          const glm::vec2& position,
                          const glm::vec2& velocity,
                          const std::string& texture_name = "xiaoyu",
                          bool isPlayerBullet = false,
                          bool isExistForever = true,
                          bool isSeg = false);
} // namespace th::BulletSystem