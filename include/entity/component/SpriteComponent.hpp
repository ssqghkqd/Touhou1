#pragma once


namespace th
{
    // 精灵组件
    struct TransformComponent
    {
        glm::vec2 position = {0.0f, 0.0f};
        float rotation = 0.0f;
        glm::vec2 scale = {1.0f, 1.0f};
    };
} // namespace th
