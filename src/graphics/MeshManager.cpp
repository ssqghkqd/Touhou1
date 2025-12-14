module;
#include <vector>
module graphics.MeshManager;
import opengl;


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

    gl::genVertexArrays(1, &mesh.vao);
    gl::genBuffers(1, &mesh.vbo);
    gl::genBuffers(1, &mesh.ebo);

    gl::bindVertexArray(mesh.vao);

    // 顶点缓冲区 (使用Vertex结构)
    gl::bindBuffer(gl::array_buffer, mesh.vbo);
    gl::bufferData(gl::array_buffer, vertices.size() * sizeof(Vertex), vertices.data(), gl::static_draw);

    // 索引缓冲区
    gl::bindBuffer(gl::element_array_buffer, mesh.ebo);
    gl::bufferData(gl::element_array_buffer, indices.size() * sizeof(unsigned int), indices.data(), gl::static_draw);

    // 位置属性 (属性索引0)
    gl::vertexAttribPointer(0, 2, gl::FLOAT, gl::FALSE, sizeof(Vertex), (void*)offsetof(Vertex, pos));
    gl::enableVertexAttribArray(0);

    // UV属性 (属性索引1)
    gl::vertexAttribPointer(1, 2, gl::FLOAT, gl::FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
    gl::enableVertexAttribArray(1);

    gl::bindVertexArray(0);
    mesh.indexCount = static_cast<int>(indices.size());

    return mesh;
}

MeshManager::~MeshManager()
{
    // 清理所有网格资源
    for (auto& [key, meshPtr] : m_meshes)
    {
        if (meshPtr->vao)
            gl::deleteVertexArrays(1, &meshPtr->vao);
        if (meshPtr->vbo)
            gl::deleteBuffers(1, &meshPtr->vbo);
        if (meshPtr->ebo)
            gl::deleteBuffers(1, &meshPtr->ebo);
    }
}
} // namespace th
