#include "graphics/ShaderManager.hpp"
#include "utils/Logger.hpp"
#include "graphics/Shader.hpp"

namespace th
{
    ShaderManager &ShaderManager::getInstance()
    {
        static ShaderManager instance;
        return instance;
    }

    const Shader &ShaderManager::loadShader(const std::string &name,
                                      const std::string &vertPath,
                                      const std::string &fragPath)
    {
        auto it = m_shaders.find(name);
        if (it != m_shaders.end())
        {
            thLogger::warning(name + "已被加载");
            return *(it->second);
        }

        auto newShader = std::make_unique<Shader>();
        if (!newShader->load(vertPath, fragPath))
        {
            thLogger::critical("着色器加载失败: " + name +
                               "\n  顶点着色器: " + vertPath +
                               "\n  片段着色器: " + fragPath);
        }

        // 插入并获取引用
        auto [iter, success] = m_shaders.emplace(name, std::move(newShader));
        return *(iter->second);
    }

    const Shader &ShaderManager::getShader(const std::string &name)
    {
        static Shader defaultShader;
        auto it = m_shaders.find(name);
        if (it != m_shaders.end())
        {
            return *(it->second);
        }
        thLogger::error("找不到:" + name);
        return defaultShader;
    }
} // namespace th
