#pragma once
#include <functional>
#include <memory>
#include <string>

#include "vec2.hpp"

namespace th
{
class MeshManager
{
  public:
    struct Vertex
    {
        glm::vec2 pos;
        glm::vec2 texCoord;
    };
    struct Mesh
    {
        unsigned int vao = 0;
        unsigned int vbo = 0;
        unsigned int ebo = 0;
        int indexCount = 0;
    };

    const Mesh& GetQuadMesh();

    // 在拷贝和移动操作中禁用
    MeshManager(const MeshManager&) = delete;
    MeshManager& operator=(const MeshManager&) = delete;
    MeshManager(MeshManager&&) = delete;
    MeshManager& operator=(MeshManager&&) = delete;

    MeshManager() = default;
    ~MeshManager();

  private:
    static Mesh CreateQuadMesh();

    std::unordered_map<std::string, std::unique_ptr<Mesh>> m_meshes;
};
} // namespace th
