module;
#include <memory>
module graphics.ShaderManager;
import spdlog;
import graphics.Shader;

namespace th
{


const Shader& ShaderManager::loadShader(const std::string& name,
                                        const std::string& vertPath,
                                        const std::string& fragPath)
{
    auto it = m_shaders.find(name);
    if (it != m_shaders.end())
    {
        spdlog::warn("{}已被加载");
        return *(it->second);
    }

    auto newShader = std::make_unique<Shader>();
    if (!newShader->load(vertPath, fragPath))
    {
        spdlog::error("着色器加载失败：{}, 顶点着色器:{}, 片段着色器{}", name, vertPath, fragPath);
    }

    // 插入并获取引用
    auto [iter, success] = m_shaders.emplace(name, std::move(newShader));
    return *(iter->second);
}

const Shader& ShaderManager::getShader(const std::string& name)
{
    static Shader defaultShader;
    auto it = m_shaders.find(name);
    if (it != m_shaders.end())
    {
        return *(it->second);
    }
    spdlog::error("{}找不到", name);
    return defaultShader;
}
} // namespace th
