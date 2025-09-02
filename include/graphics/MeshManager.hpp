#pragma once


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
            GLuint vao = 0;
            GLuint vbo = 0;
            GLuint ebo = 0;
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

        const Mesh &GetOrCreateMesh(const std::string &key, std::function<Mesh()> createFunc);
        Mesh CreateQuadMesh();
        Mesh CreateCircleMesh(int segments);

        std::unordered_map<std::string, std::unique_ptr<Mesh>> m_meshes;
    };
} // namespace th
