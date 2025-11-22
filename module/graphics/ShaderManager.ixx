//
// Created by ss on 2025/11/22.
//
module;
#include <memory>
#include <unordered_map>
#include <string>
export module graphics.ShaderManager;
import graphics.Shader;

export namespace th
{
class ShaderManager
{
public:

    // 加载 获取着色器
    const Shader &loadShader(const std::string &name,
                             const std::string &vertName,
                             const std::string &fragName);

    const Shader &getShader(const std::string &name);

    // 删除拷贝构造和赋值操作符
    ShaderManager(const ShaderManager &) = delete;
    ShaderManager &operator=(const ShaderManager &) = delete;


    ShaderManager() = default;
    ~ShaderManager() = default;

private:
    std::unordered_map<std::string, std::unique_ptr<Shader>> m_shaders;
};
} // namespace th
