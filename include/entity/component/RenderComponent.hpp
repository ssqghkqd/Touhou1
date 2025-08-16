#pragma once

#include "stdafx.hpp"

namespace th
{
    enum class MeshType
    {
        Rect,
        Circle
    };

    struct RenderComponent
    {
        std::string textureName;             // 纹理在TextureManager中的标识符
        MeshType meshType = MeshType::Rect; // 默认为四边形
        bool isStatic = false;              // 是否静态实体（可缓存矩阵）
        glm::mat4 cachedMatrix;             // 静态实体的缓存矩阵
        bool isVisible = true;
        glm::vec2 size = {1.0f, 1.0f};
    };
} // namespace th
