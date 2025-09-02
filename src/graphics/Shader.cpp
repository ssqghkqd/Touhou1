#include "graphics/Shader.hpp"
#include "utils/FileManager.hpp"
#include "utils/Logger.hpp"

#define aboGetUniformLocation glGetUniformLocation

namespace fs = std::filesystem;

namespace th
{
    void Shader::shutdown()
    {
    }

    bool Shader::use() const
    {
        if (m_id)
        {
            glUseProgram(m_id);
            return true;
        }
        return false;
    }

    // 在shaders下的相对路径
    bool Shader::load(const std::string &vertPath, const std::string &fragPath)
    {
        m_vertPath = vertPath;
        m_fragPath = fragPath;
        // 获取完整路径
        fs::path vertFullPath = FileManager::getResourcePath("shaders") / vertPath;
        fs::path fragFullPath = FileManager::getResourcePath("shaders") / fragPath;

        // 读取着色器源代码
        std::string vertexShaderSource, fragmentShaderSource;
        try
        {
            vertexShaderSource = FileManager::readFile(vertFullPath);
            fragmentShaderSource = FileManager::readFile(fragFullPath);
        }
        catch (const std::exception &e)
        {
            std::string s = e.what();
            thLogger::critical("着色器代码读取失败" + s);
            return false;
        }

        // 创建着色器并检查
        vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
        fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
        shaders.push_back(vertexShader);
        shaders.push_back(fragmentShader);
        // 创建链接着色器程序并检查 删除着色器
        createShaderProgram(shaders);
        return true;
    }

    // 创建着色器并检查
    GLuint Shader::createShader(GLenum type, std::string &shaderSource)
    {
        const char *shaderSource_c = shaderSource.c_str();
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &shaderSource_c, NULL);
        glCompileShader(shader);
        checkCompileShader(shader, type);
        return shader;
    }

    // 检查着色器是否编译成功
    void Shader::checkCompileShader(GLuint shader, GLenum type)
    {
        int length = 0;
        int success;
        std::string typeName;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        std::string shaderLog(length, '\0');
        switch (type)
        {
        case GL_VERTEX_SHADER:
            typeName = "顶点";
            break;
        case GL_FRAGMENT_SHADER:
            typeName = "片段";
            break;
        case GL_GEOMETRY_SHADER:
            typeName = "几何";
            break;
        default:
            typeName = "未知";
        }

        if (!success)
        {
            glGetShaderInfoLog(shader, length, NULL, shaderLog.data());
            shaderLog.pop_back();
            thLogger::error(typeName + "着色器编译失败:" + shaderLog);
        }
    }

    // 创建 链接着色器程序并检查
    void Shader::createShaderProgram(const std::vector<GLuint> &shaderList)
    {
        // 创建着色器程序
        m_id = glCreateProgram();
        for (const auto &shader : shaderList)
        {
            glAttachShader(m_id, shader);
        }
        glLinkProgram(m_id);
        // 检查成功
        checkShaderProgram(m_id);
        // 删除着色器
        for (const auto &shader : shaderList)
        {
            glDeleteShader(shader);
        }
    }

    // 检查着色器程序链接是否成功
    void Shader::checkShaderProgram(GLuint shaderProgram)
    {
        int length = 0;
        int success = 0;
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &length);
        std::string programLog(length, '\0');
        if (!success)
        {
            glGetProgramInfoLog(shaderProgram, length, NULL, programLog.data());
            programLog.pop_back(); // 移除末尾的null终止符
            thLogger::critical("着色器程序链接失败：" + programLog);
        }
    }

    bool Shader::set(const std::string &name, const glm::mat4 &mat) const
    {
        if (m_id)
        {
            GLint location = aboGetUniformLocation(m_id, name.c_str());
            if (location >= 0) // 如果找不到，返回-1
            {
                glUniformMatrix4fv(location, 1.0f, GL_FALSE, glm::value_ptr(mat));
                return true;
            }
            thLogger::error("找不到" + name);
            return false;
        }
        thLogger::error("着色器程序未使用");
        return false;
    }

    bool Shader::set(const std::string &name, const int num) const
    {
        if (m_id)
        {
            GLint location = aboGetUniformLocation(m_id, name.c_str());
            if (location >= 0) // 如果找不到，返回-1
            {
                glUniform1i(location, num);
                return true;
            }
            thLogger::error("找不到" + name);
            return false;
        }
        thLogger::error("着色器程序未使用");
        return false;
    }
} // namespace th
