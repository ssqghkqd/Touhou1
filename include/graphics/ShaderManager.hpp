#pragma once

#include <unordered_map>
#include <string>

namespace th
{
    class Shader;

    class ShaderManager
    {
    public:
        // 获取实例
        static ShaderManager &getInstance();

        // 加载 获取着色器
        const Shader &loadShader(const std::string &name,
                                 const std::string &vertName,
                                 const std::string &fragName);

        const Shader &getShader(const std::string &name);

        // 删除拷贝构造和赋值操作符
        ShaderManager(const ShaderManager &) = delete;
        ShaderManager &operator=(const ShaderManager &) = delete;

        // 移动语义也删除（单例不需要）
        ShaderManager(ShaderManager &&) = delete;
        ShaderManager &operator=(ShaderManager &&) = delete;

    private:
        ShaderManager() = default;
        ~ShaderManager() = default;
        std::unordered_map<std::string, std::unique_ptr<Shader>> m_shaders;
    };
} // namespace th
