//
// Created by ss on 2025/11/22.
//
module;
#include <string>
#include <glm/ext.hpp>
#include <entt/entt.hpp>
export module game.system.BulletSys;

export namespace th::BulletSys
{
entt::entity createBullet(entt::registry& registry,
                          const glm::vec2& position,
                          const glm::vec2& velocity,
                          const std::string& configName = "bullet_default",
                          bool isPlayerBullet = false);
} // namespace th::BulletSystem
