#include "graphics/MeshManager.hpp"

namespace th
{
    MeshManager &MeshManager::getInstance()
    {
        static MeshManager instance;
        return instance;
    }

    const MeshManager::Mesh &MeshManager::GetQuadMesh()
    {
        static const std::string quadKey = "quad";
        return GetOrCreateMesh(quadKey, [this]()
                               { return CreateQuadMesh(); });
    }

    const MeshManager::Mesh &MeshManager::GetCircleMesh(int segments)
    {
        std::string key = "circle_" + std::to_string(segments);
        return GetOrCreateMesh(key, [this, segments]()
                               { return CreateCircleMesh(segments); });
    }

    // ======================= 私有方法 =======================

    const MeshManager::Mesh &MeshManager::GetOrCreateMesh(
        const std::string &key,
        const std::function<Mesh()>& createFunc)
    {
        auto it = m_meshes.find(key);
        if (it != m_meshes.end())
        {
            thLogger::info(key + "已被创建");
            return *(it->second);
        }

        // 创建新网格
        Mesh newMesh = createFunc();
        auto meshPtr = std::make_unique<Mesh>(std::move(newMesh));
        auto [insertedIt, success] = m_meshes.emplace(key, std::move(meshPtr));

        if (!success)
        {
            thLogger::critical("网格创建失败");
        }

        return *(insertedIt->second);
    }

    MeshManager::Mesh MeshManager::CreateQuadMesh()
    {
        Mesh mesh;

        // 顶点数据 (位置 + UV)
        std::vector<Vertex> vertices = {
            // 位置          // UV
            {{-0.5f, -0.5f}, {0.0f, 0.0f}}, // 左下
            {{0.5f, -0.5f}, {1.0f, 0.0f}},  // 右下
            {{0.5f, 0.5f}, {1.0f, 1.0f}},   // 右上
            {{-0.5f, 0.5f}, {0.0f, 1.0f}}   // 左上
        };

        // 索引数据
        std::vector<unsigned int> indices = {
            0, 1, 2, // 第一个三角形
            2, 3, 0  // 第二个三角形
        };

        // 创建OpenGL对象
        glGenVertexArrays(1, &mesh.vao);
        glGenBuffers(1, &mesh.vbo);
        glGenBuffers(1, &mesh.ebo);

        glBindVertexArray(mesh.vao);

        // 顶点缓冲区 (使用Vertex结构)
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                     vertices.data(), GL_STATIC_DRAW);

        // 索引缓冲区
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                     indices.data(), GL_STATIC_DRAW);

        // 位置属性 (属性索引0)
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, pos));
        glEnableVertexAttribArray(0);

        // UV属性 (属性索引1)
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)offsetof(Vertex, texCoord));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
        mesh.indexCount = static_cast<int>(indices.size());

        return mesh;
    }

    MeshManager::Mesh MeshManager::CreateCircleMesh(int segments)
    {
        Mesh mesh;

        if (segments < 3)
            segments = 3; // 最少3个分段

        std::vector<Vertex> vertices;

        // 中心点
        vertices.push_back({{0.0f, 0.0f}, {0.5f, 0.5f}});

        // 圆周点
        for (int i = 0; i <= segments; i++)
        {
            float angle = 2.0f * glm::pi<float>() * i / segments;
            float x = std::cos(angle);
            float y = std::sin(angle);

            // 径向UV映射
            vertices.push_back({{x, y}, {0.5f + 0.5f * x, 0.5f + 0.5f * y}});
        }

        // 索引数据 (三角形扇)
        std::vector<unsigned int> indices;
        indices.reserve(segments * 3);

        for (int i = 0; i < segments; i++)
        {
            indices.push_back(0);     // 中心点
            indices.push_back(i + 1); // 当前点
            indices.push_back(i + 2); // 下一个点
        }

        // 创建OpenGL对象
        glGenVertexArrays(1, &mesh.vao);
        glGenBuffers(1, &mesh.vbo);
        glGenBuffers(1, &mesh.ebo);

        glBindVertexArray(mesh.vao);

        // 顶点缓冲区 (使用Vertex结构)
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                     vertices.data(), GL_STATIC_DRAW);

        // 索引缓冲区
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                     indices.data(), GL_STATIC_DRAW);

        // 位置属性
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)offsetof(Vertex, pos));
        glEnableVertexAttribArray(0);

        // UV属性
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                              (void *)offsetof(Vertex, texCoord));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
        mesh.indexCount = static_cast<int>(indices.size());

        return mesh;
    }

    MeshManager::~MeshManager()
    {
        // 清理所有网格资源
        for (auto &[key, meshPtr] : m_meshes)
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
