module;
#include <filesystem>
#include <glm/ext.hpp>
module graphics.Shader;
import utils.FileManager;
import spdlog;

namespace fs = std::filesystem;

namespace th
{

void Shader::use() const
{
    gl::useProgram(m_id);
}

// 在shaders下的相对路径
bool Shader::load(const std::string& vertPath, const std::string& fragPath)
{
    m_vertPath = vertPath;
    m_fragPath = fragPath;
    // 获取完整路径
    const fs::path vertFullPath = FileManager::getResourcePath("shaders", false) / vertPath;
    const fs::path fragFullPath = FileManager::getResourcePath("shaders", false) / fragPath;

    // 读取着色器源代码
    std::string vertexShaderSource, fragmentShaderSource;
    try
    {
        vertexShaderSource = FileManager::readFile(vertFullPath);
        fragmentShaderSource = FileManager::readFile(fragFullPath);
    }
    catch (const std::exception& e)
    {
        std::string s = e.what();
        spdlog::critical("着色器代码编译错误:{}", s);
        throw;
    }

    // 创建着色器并检查
    m_vertexShader = createShader(gl::vertex_shader, vertexShaderSource);
    m_fragmentShader = createShader(gl::fragment_shader, fragmentShaderSource);
    m_shaders.push_back(m_vertexShader);
    m_shaders.push_back(m_fragmentShader);
    // 创建链接着色器程序并检查 删除着色器
    createShaderProgram(m_shaders);
    return true;
}

// 创建着色器并检查
gl::uint Shader::createShader(const gl::glenum type, const std::string& shaderSource)
{
    const char* shaderSource_c = shaderSource.c_str();
    gl::uint shader = gl::createShader(type);
    gl::shaderSource(shader, 1, &shaderSource_c, nullptr);
    gl::compileShader(shader);
    checkCompileShader(shader, type);
    return shader;
}

// 检查着色器是否编译成功
void Shader::checkCompileShader(gl::uint shader, gl::glenum type)
{
    int length = 0;
    int success;
    std::string typeName;
    gl::getShaderiv(shader, gl::compile_status, &success);
    gl::getShaderiv(shader, gl::info_log_length, &length);
    std::string shaderLog(length, '\0');
    switch (type)
    {
        case gl::vertex_shader:
            typeName = "顶点";
            break;
        case gl::fragment_shader:
            typeName = "片段";
            break;
        case gl::geometry_shader:
            typeName = "几何";
            break;
        default:
            typeName = "未知";
    }

    if (!success)
    {
        gl::getShaderInfoLog(shader, length, nullptr, shaderLog.data());
        shaderLog.pop_back();
        spdlog::error("{}着色器编译失败:{}", typeName, shaderLog);
    }
}

void Shader::createShaderProgram(const std::vector<gl::uint>& shaderList)
{
    m_id = gl::createProgram();
    for (const auto& shader : shaderList)
    {
        gl::attachShader(m_id, shader);
    }
    gl::linkProgram(m_id);
    checkShaderProgram(m_id);
    for (const auto& shader : shaderList)
    {
        gl::deleteShader(shader);
    }
}

void Shader::checkShaderProgram(gl::uint shaderProgram)
{
    int length = 0;
    int success = 0;
    gl::getProgramiv(shaderProgram, gl::link_status, &success);
    gl::getProgramiv(shaderProgram, gl::info_log_length, &length);
    std::string programLog(length, '\0');
    if (!success)
    {
        gl::getProgramInfoLog(shaderProgram, length, nullptr, programLog.data());
        programLog.pop_back();
        spdlog::critical("着色器链接失败:{}", programLog);
        throw std::runtime_error("着色器链接失败");
    }
}

void Shader::set(const std::string& name, const glm::mat4& mat) const
{
    const gl::glint location = gl::getUniformLocation(m_id, name.c_str());
    assert(location >= 0 && "找不到uniform！");
    gl::uniformMatrix4fv(location, 1, gl::FALSE, glm::value_ptr(mat));
}

void Shader::set(const std::string& name, int num) const
{
    const gl::glint location = gl::getUniformLocation(m_id, name.c_str());
    assert(location >= 0 && "找不到uniform！");
    gl::uniform1i(location, num);
}

void Shader::set(const std::string& name, const glm::vec4& vec4) const
{
    const gl::glint location = gl::getUniformLocation(m_id, name.c_str());
    assert(location >= 0 && "找不到uniform！");
    gl::uniform4f(location, vec4.x, vec4.y, vec4.z, vec4.t);
}
} // namespace th
