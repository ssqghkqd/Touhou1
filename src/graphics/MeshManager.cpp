#include "graphics/MeshManager.hpp"

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "ext.hpp"
#include "glad.h"
#include "spdlog/spdlog.h"

namespace th
{

const MeshManager::Mesh& MeshManager::GetQuadMesh()
{
    static const Mesh quadMesh = CreateQuadMesh(); // 静态初始化，只创建一次
    return quadMesh;
}

// ======================= 私有方法 =======================

MeshManager::Mesh MeshManager::CreateQuadMesh()
{
    Mesh mesh;

    // 顶点数据 (位置 + UV)
    const std::vector<Vertex> vertices = {
        // 位置          // UV
        {{-0.5f, -0.5f}, {0.0f, 0.0f}}, // 左下
        {{0.5f, -0.5f}, {1.0f, 0.0f}},  // 右下
        {{0.5f, 0.5f}, {1.0f, 1.0f}},   // 右上
        {{-0.5f, 0.5f}, {0.0f, 1.0f}}   // 左上
    };

    // 索引数据
    const std::vector<unsigned int> indices = {
        0,
        1,
        2, // 第一个三角形
        2,
        3,
        0 // 第二个三角形
    };

    // 创建OpenGL对象
    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vbo);
    glGenBuffers(1, &mesh.ebo);

    glBindVertexArray(mesh.vao);

    // 顶点缓冲区 (使用Vertex结构)
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // 索引缓冲区
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    // 位置属性 (属性索引0)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    glEnableVertexAttribArray(0);

    // UV属性 (属性索引1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    mesh.indexCount = static_cast<int>(indices.size());

    return mesh;
}

MeshManager::~MeshManager()
{
    // 清理所有网格资源
    for (auto& [key, meshPtr] : m_meshes)
    {
        if (meshPtr->vao)
            glDeleteVertexArrays(1, &meshPtr->vao);
        if (meshPtr->vbo)
            glDeleteBuffers(1, &meshPtr->vbo);
        if (meshPtr->ebo)
            glDeleteBuffers(1, &meshPtr->ebo);
    }
}
} // namespace th
