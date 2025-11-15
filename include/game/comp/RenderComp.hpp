#pragma once
#include <glm.hpp>
#include <string>

namespace th
{
struct RenderComp
{
    std::string textureName;
    glm::vec4 color = glm::vec4(1.0f);
    glm::vec2 size = glm::vec2(0.0f);
    bool isVisible = true;
};
} // namespace th