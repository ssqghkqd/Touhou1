#pragma once


namespace th
{
    // 精灵组件
    struct SpriteComponent
    {
        glm::vec2 position = {0.0f, 0.0f};
        float rotation = 0.0f;
        glm::vec2 scale = {1.0f, 1.0f};
        glm::vec2 velocity = {0.0f, 0.0f};
    };
} // namespace th
