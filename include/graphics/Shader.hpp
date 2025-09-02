#pragma once


namespace th
{
    class Shader
    {
    public:
        Shader() = default;
        ~Shader() = default;
        bool load(const std::string &vertPath, const std::string &fragPath);
        GLuint createShader(GLenum type, std::string &shaderSource);
        void checkCompileShader(GLuint shader, GLenum type);
        void createShaderProgram(const std::vector<GLuint> &shaderList);
        void checkShaderProgram(GLuint shaderProgram);

        bool use() const;
        void shutdown();

        inline GLuint getID() const { return m_id; }

        // 设置uniform
        bool set(const std::string &name, const glm::mat4 &mat) const;
        bool set(const std::string &name, const int num) const;

    private:
        // 着色器程序ID
        GLuint m_id = 0;
        GLuint vertexShader = 0;
        GLuint fragmentShader = 0;
        std::string m_vertPath;
        std::string m_fragPath;

        std::vector<GLuint> shaders;
    };
} // namespace th
