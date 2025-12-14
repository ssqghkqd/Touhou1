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
    [[nodiscard]] bool load(const std::string &vertPath, const std::string &fragPath);
    gl::uint createShader(gl::glenum type, const std::string &shaderSource);
    static void checkCompileShader(gl::uint shader, gl::glenum type);
    void createShaderProgram(const std::vector<gl::uint> &shaderList);
    static void checkShaderProgram(gl::uint shaderProgram);

    void use() const;

    [[nodiscard]] inline gl::uint getID() const { return m_id; }

    // 设置uniform
    void set(const std::string &name, const glm::mat4 &mat) const;
    void set(const std::string &name, int num) const;
    void set(const std::string &name, const glm::vec4& vec4) const;

private:
    // 着色器程序ID
    gl::uint m_id = 0;
    gl::uint m_vertexShader = 0;
    gl::uint m_fragmentShader = 0;
    std::string m_vertPath;
    std::string m_fragPath;

    std::vector<gl::uint> m_shaders;
};
} // namespace th
