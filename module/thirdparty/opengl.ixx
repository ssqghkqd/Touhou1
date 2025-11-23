//
// Created by ss on 2025/11/22.
//
module;
#include <glad/glad.h>

export module opengl;

export namespace gl
{
auto& loadGLLoader = gladLoadGLLoader;
auto& viewport = glViewport;
auto& genVertexArrays = glGenVertexArrays;
auto& bindVertexArray = glBindVertexArray;
auto& genBuffers = glGenBuffers;
auto& bindBuffer = glBindBuffer;
auto& bufferData = glBufferData;
auto& vertexAttribPointer = glVertexAttribPointer;
auto& enableVertexAttribArray = glEnableVertexAttribArray;
auto& deleteVertexArrays = glDeleteVertexArrays;
auto& deleteBuffers = glDeleteBuffers;
auto& enable = glEnable;
auto& disable = glDisable;
auto& blendFunc = glBlendFunc;
auto& clearColor = glClearColor;
auto& activeTexture = glActiveTexture;
auto& drawElements = glDrawElements;
auto& clear = glClear;
auto& useProgram = glUseProgram;
auto& createShader = glCreateShader;
auto& shaderSource = glShaderSource;
auto& compileShader = glCompileShader;
auto& getShaderiv = glGetShaderiv;
auto& getShaderInfoLog = glGetShaderInfoLog;
auto& createProgram = glCreateProgram;
auto& attachShader = glAttachShader;
auto& linkProgram = glLinkProgram;
auto& deleteShader = glDeleteShader;
auto& getProgramiv = glGetProgramiv;
auto& getProgramInfoLog = glGetProgramInfoLog;
auto& getUniformLocation = glGetUniformLocation;
auto& uniformMatrix4fv = glUniformMatrix4fv;
auto& uniform1i = glUniform1i;
auto& uniform4f = glUniform4f;
auto& genTextures = glGenTextures;
auto& bindTexture = glBindTexture;
auto& texParameteri = glTexParameteri;
auto& texImage2D = glTexImage2D;
auto& generateMipmap = glGenerateMipmap;
auto& deleteTextures = glDeleteTextures;

// 先占个坑 后续再加更多需要的符号
using loadproc = GLADloadproc;
using glenum = GLenum;
using uint = GLuint;
using glint = GLint;

constexpr auto ARRAY_BUFFER = GL_ARRAY_BUFFER;
constexpr auto ELEMENT_ARRAY_BUFFER = GL_ELEMENT_ARRAY_BUFFER;
constexpr auto STATIC_DRAW = GL_STATIC_DRAW;
constexpr auto FLOAT = GL_FLOAT;
constexpr auto FALSE = GL_FALSE;
constexpr auto BLEND = GL_BLEND;
constexpr auto SRC_ALPHA = GL_SRC_ALPHA;
constexpr auto ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA;
constexpr auto DEPTH_TEST = GL_DEPTH_TEST;
constexpr auto TEXTURE0 = GL_TEXTURE0;
constexpr auto TRIANGLES = GL_TRIANGLES;
constexpr auto UNSIGNED_INT = GL_UNSIGNED_INT;
constexpr auto COLOR_BUFFER_BIT = GL_COLOR_BUFFER_BIT;
constexpr auto VERTEX_SHADER = GL_VERTEX_SHADER;
constexpr auto FRAGMENT_SHADER = GL_FRAGMENT_SHADER;
constexpr auto COMPILE_STATUS = GL_COMPILE_STATUS;
constexpr auto INFO_LOG_LENGTH = GL_INFO_LOG_LENGTH;
constexpr auto GEOMETRY_SHADER = GL_GEOMETRY_SHADER;
constexpr auto LINK_STATUS = GL_LINK_STATUS;
constexpr auto TEXTURE_2D = GL_TEXTURE_2D;
constexpr auto TEXTURE_WRAP_S = GL_TEXTURE_WRAP_S;
constexpr auto TEXTURE_WRAP_T = GL_TEXTURE_WRAP_T;
constexpr auto TEXTURE_MIN_FILTER = GL_TEXTURE_MIN_FILTER;
constexpr auto TEXTURE_MAG_FILTER = GL_TEXTURE_MAG_FILTER;
constexpr auto REPEAT = GL_REPEAT;
constexpr auto LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR;
constexpr auto LINEAR = GL_LINEAR;
constexpr auto RGB = GL_RGB;
constexpr auto RED = GL_RED;
constexpr auto RGBA = GL_RGBA;
constexpr auto UNSIGNED_BYTE = GL_UNSIGNED_BYTE;
} // namespace gl

export namespace gf
{
auto& drawElements = glDrawElements;
}
