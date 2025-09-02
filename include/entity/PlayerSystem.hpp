#pragma once

#include "entt/entt.hpp"

namespace th
{
    class PlayerSystem
    {
    public:
        static PlayerSystem &getInstance();
        void init();
        void updateMove(entt::registry &registry);
        void toggleNoHitbox(entt::registry &registry);
        entt::entity createPlayer(entt::registry &registry);

    private:
        PlayerSystem() = default;
        ~PlayerSystem() = default;
        bool inited = false;
    };
} // namespace th
