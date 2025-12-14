//
// Created by ss on 2025/11/22.
//
module;
#include <string>
#include <glm/ext.hpp>
export module game.comp:render;
export namespace th
{
struct RenderComp
{
    std::string textureName;
    glm::vec4 color = glm::vec4(1.0f);
    glm::vec2 size = glm::vec2(0.0f);
    bool isVisible = true;
};
} // namespace th
