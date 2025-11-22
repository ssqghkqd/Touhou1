module;
#include <filesystem>
#include <glm/ext.hpp>
module graphics.Shader;
import utils.FileManager;
import spdlog;

namespace fs = std::filesystem;

namespace th
{

bool Shader::use() const
{
    if (m_id)
    {
        gl::useProgram(m_id);
        return true;
    }
    return false;
}

// 在shaders下的相对路径
bool Shader::load(const std::string& vertPath, const std::string& fragPath)
{
    m_vertPath = vertPath;
    m_fragPath = fragPath;
    // 获取完整路径
    fs::path vertFullPath = FileManager::getResourcePath("shaders", false) / vertPath;
    fs::path fragFullPath = FileManager::getResourcePath("shaders", false) / fragPath;

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
        return false;
    }

    // 创建着色器并检查
    vertexShader = createShader(gl::VERTEX_SHADER, vertexShaderSource);
    fragmentShader = createShader(gl::FRAGMENT_SHADER, fragmentShaderSource);
    shaders.push_back(vertexShader);
    shaders.push_back(fragmentShader);
    // 创建链接着色器程序并检查 删除着色器
    createShaderProgram(shaders);
    return true;
}

// 创建着色器并检查
gl::uint Shader::createShader(gl::glenum type, std::string& shaderSource)
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
    gl::getShaderiv(shader, gl::COMPILE_STATUS, &success);
    gl::getShaderiv(shader, gl::INFO_LOG_LENGTH, &length);
    std::string shaderLog(length, '\0');
    switch (type)
    {
        case gl::VERTEX_SHADER:
            typeName = "顶点";
            break;
        case gl::FRAGMENT_SHADER:
            typeName = "片段";
            break;
        case gl::GEOMETRY_SHADER:
            typeName = "几何";
            break;
        default:
            typeName = "未知";
    }

    if (!success)
    {
        gl::getShaderInfoLog(shader, length, nullptr, shaderLog.data());
        shaderLog.pop_back();
        spdlog::error("着色器编译失败:{}", shaderLog);
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
    gl::getProgramiv(shaderProgram, gl::LINK_STATUS, &success);
    gl::getProgramiv(shaderProgram, gl::INFO_LOG_LENGTH, &length);
    std::string programLog(length, '\0');
    if (!success)
    {
        gl::getProgramInfoLog(shaderProgram, length, nullptr, programLog.data());
        programLog.pop_back();
        spdlog::critical("着色器链接失败:{}", programLog);
        throw;
    }
}

bool Shader::set(const std::string& name, const glm::mat4& mat) const
{
    if (m_id)
    {
        gl::glint location = gl::getUniformLocation(m_id, name.c_str());
        if (location >= 0)
        {
            gl::uniformMatrix4fv(location, 1, gl::FALSE, glm::value_ptr(mat));
            return true;
        }
        spdlog::error("找不到:{}", location);
        return false;
    }
    spdlog::error("着色器程序未使用:{}", name);
    return false;
}

bool Shader::set(const std::string& name, const int num) const
{
    if (m_id)
    {
        gl::glint location = gl::getUniformLocation(m_id, name.c_str());
        if (location >= 0)
        {
            gl::uniform1i(location, num);
            return true;
        }
        spdlog::error("找不到:{}", location);
        return false;
    }
    spdlog::error("着色器程序未使用:{}", name);
    return false;
}

bool Shader::set(const std::string& name, const glm::vec4& vec4) const
{
    if (m_id)
    {
        gl::glint location = gl::getUniformLocation(m_id, name.c_str());
        if (location >= 0)
        {
            gl::uniform4f(location,vec4.x,vec4.y,vec4.z,vec4.t);
            return true;
        }
        spdlog::error("找不到:{}", location);
        return false;
    }
    spdlog::error("着色器程序未使用:{}", name);
    return false;
}
} // namespace th
