#pragma once
#include "vec2.hpp"
#include <string>
#include <functional>

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

        static MeshManager &getInstance();

        const Mesh &GetQuadMesh();
        const Mesh &GetCircleMesh(int segments = 16);

        // 在拷贝和移动操作中禁用
        MeshManager(const MeshManager &) = delete;
        MeshManager &operator=(const MeshManager &) = delete;
        MeshManager(MeshManager &&) = delete;
        MeshManager &operator=(MeshManager &&) = delete;

    private:
        MeshManager() = default;
        ~MeshManager();

        const Mesh &GetOrCreateMesh(const std::string &key, const std::function<Mesh()>& createFunc);
        static Mesh CreateQuadMesh();
        static Mesh CreateCircleMesh(int segments);

        std::unordered_map<std::string, std::unique_ptr<Mesh>> m_meshes;
    };
} // namespace th
