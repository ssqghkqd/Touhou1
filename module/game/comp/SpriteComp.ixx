//
// Created by ss on 2025/11/22.
//

module;
#include <glm/ext.hpp>
export module game.comp:sprite;

export namespace th
{
struct SpriteComp
{
    glm::vec2 velocity = glm::vec2(0.0f);
};
} // namespace th