#pragma once


namespace th
{
    struct PlayerTag
    {
    };
    struct PlayerControl
    {
        float moveSpeed = 548.57f;
        float slowMoveSpeed = 300.02f;
        bool slowMode = false;

        // 当前速度向量
        glm::vec2 velocity = {0.0f, 0.0f};
        float smoothFactor = 0.7f; // 0.0-1.0，值越大响应越快
        glm::vec2 targetDir = {0.0f, 0.0f};
    };
    struct HitboxComponent
    {
        glm::vec2 offset = {0.0f, 3.0f}; // 相对实体位置的偏移
        float radius = 2.0f;
        bool isActive = true; // 是否启用碰撞(debug)

        std::string textureName = "hitbox";
    };
} // namespace th
