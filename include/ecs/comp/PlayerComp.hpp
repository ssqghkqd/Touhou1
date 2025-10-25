#pragma once
#include "vec2.hpp"

namespace th
{

struct PlayerComp
{
    bool isSlow = false;
    glm::vec2 hitboxOffset = {0.0f, -3.0f};
    glm::vec2 hitboxSize = {10.0f, 10.0f};
    glm::vec2 targetDir = {0.0f, 0.0f};
};
} // namespace th