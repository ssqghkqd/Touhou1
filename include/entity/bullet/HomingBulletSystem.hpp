#pragma once

#include "entt/entt.hpp"


namespace th
{
class HomingBulletSystem
{
  public:
    void init(entt::registry& registry);
    static HomingBulletSystem& getInstance();

    void createHoming(entt::registry& registry, int way = 1, float angle = 0.0f);

  private:
    HomingBulletSystem() = default;
    ~HomingBulletSystem() = default;
    bool inited = false;
    entt::entity m_player;
    glm::vec2 m_playerPosition;
};
} // namespace th
