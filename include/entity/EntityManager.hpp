#pragma once

#include "entt/entt.hpp"

namespace th
{
    class EntityManager
    {
        public:
            static EntityManager &getInstance();
            void init(entt::registry &registry);
        private:
            EntityManager() = default;
            ~EntityManager() = default;

            bool inited = false;
    };
} // namespace th
