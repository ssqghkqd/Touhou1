//
// Created by ss on 2025/11/22.
//
module;
#include <glm/ext.hpp>
export module game.comp:player;

export namespace th
{

struct PlayerComp
{
    bool isSlow = false;
    glm::vec2 hitboxOffset = {0.0f, -3.0f};
    glm::vec2 hitboxSize = {20.0f, 20.0f};
    glm::vec2 targetDir = {0.0f, 0.0f};
};
} // namespace th
