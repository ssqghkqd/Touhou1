#pragma once
#include "vec2.hpp"

namespace th
{
struct EnemyComp
{
    float hp = 100.0f;
    float move_cd = 8.0f;
    glm::vec2 target_pos;
};
}