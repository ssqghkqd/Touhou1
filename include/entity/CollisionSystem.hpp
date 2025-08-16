#pragma once

#include "stdafx.hpp"
#include "entt/entt.hpp"

namespace th
{
    class CollisionSystem
    {
    public:
        static CollisionSystem &getInstance();
        void init(entt::registry &registry);
        void update(entt::registry &registry);

    private:
        CollisionSystem() = default;
        ~CollisionSystem() = default;

        bool inited = false;
        // 玩家实体（缓存提高性能）
        entt::entity m_player = entt::null;

        // 玩家碰撞数据（避免每帧重复获取）
        glm::vec2 m_playerPosition = {0.0f, 0.0f};
        float m_playerRadius = 0.0f;

        // 碰撞检测方法
        bool checkCollision(const glm::vec2 &posA, float radiusA,
                            const glm::vec2 &posB, float radiusB);
    };
} // namespace th
