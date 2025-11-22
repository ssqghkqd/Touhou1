//
// Created by ss on 2025/11/22.
//
module;
#include <entt/entt.hpp>
export module game.system.PlayerSys;

export namespace th::PlayerSys
{
void update(entt::registry& registry, float dt);
entt::entity createPlayer(entt::registry& registry);
void shot(entt::registry& registry);
entt::entity& getPlayer();
void updatePlayerMovement(entt::registry& registry);

} // namespace PlayerSystem
