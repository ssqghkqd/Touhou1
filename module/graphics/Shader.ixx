//
// Created by ss on 2025/11/22.
//
module;
#include <string>
#include <glm/ext.hpp>
#include <vector>
export module graphics.Shader;
import opengl;

export namespace th
{
class Shader
{
public:
    Shader() = default;
    ~Shader() = default;
    bool load(const std::string &vertPath, const std::string &fragPath);
    gl::uint createShader(gl::glenum type, std::string &shaderSource);
    void checkCompileShader(gl::uint shader, gl::glenum type);
    void createShaderProgram(const std::vector<gl::uint> &shaderList);
    void checkShaderProgram(gl::uint shaderProgram);

    bool use() const;

    inline gl::uint getID() const { return m_id; }

    // 设置uniform
    bool set(const std::string &name, const glm::mat4 &mat) const;
    bool set(const std::string &name, const int num) const;
    bool set(const std::string &name, const glm::vec4& vec4) const;

private:
    // 着色器程序ID
    gl::uint m_id = 0;
    gl::uint vertexShader = 0;
    gl::uint fragmentShader = 0;
    std::string m_vertPath;
    std::string m_fragPath;

    std::vector<gl::uint> shaders;
};
} // namespace th
