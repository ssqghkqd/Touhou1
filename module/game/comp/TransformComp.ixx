//
// Created by ss on 2025/11/22.
//

module;
#include <glm/ext.hpp>
export module game.comp:transform;

export namespace th
{
struct TransformComp
{
    glm::vec2 position = {0.0f, 0.0f};
    float rotation = 0.0f;
    glm::vec2 scale = {1.0f, 1.0f};
};
} // namespace th
