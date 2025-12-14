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

constexpr auto array_buffer = GL_ARRAY_BUFFER;
constexpr auto element_array_buffer = GL_ELEMENT_ARRAY_BUFFER;
constexpr auto static_draw = GL_STATIC_DRAW;
constexpr auto FLOAT = GL_FLOAT;
constexpr auto FALSE = GL_FALSE;
constexpr auto blend = GL_BLEND;
constexpr auto src_alpha = GL_SRC_ALPHA;
constexpr auto one_minus_src_alpha = GL_ONE_MINUS_SRC_ALPHA;
constexpr auto depth_test = GL_DEPTH_TEST;
constexpr auto texture0 = GL_TEXTURE0;
constexpr auto triangles = GL_TRIANGLES;
constexpr auto unsigned_int = GL_UNSIGNED_INT;
constexpr auto color_buffer_bit = GL_COLOR_BUFFER_BIT;
constexpr auto vertex_shader = GL_VERTEX_SHADER;
constexpr auto fragment_shader = GL_FRAGMENT_SHADER;
constexpr auto compile_status = GL_COMPILE_STATUS;
constexpr auto info_log_length = GL_INFO_LOG_LENGTH;
constexpr auto geometry_shader = GL_GEOMETRY_SHADER;
constexpr auto link_status = GL_LINK_STATUS;
constexpr auto texture_2d = GL_TEXTURE_2D;
constexpr auto texture_wrap_s = GL_TEXTURE_WRAP_S;
constexpr auto texture_warp_t = GL_TEXTURE_WRAP_T;
constexpr auto texture_min_filter = GL_TEXTURE_MIN_FILTER;
constexpr auto texture_mag_filter = GL_TEXTURE_MAG_FILTER;
constexpr auto repeat = GL_REPEAT;
constexpr auto linear_mimap_linear = GL_LINEAR_MIPMAP_LINEAR;
constexpr auto linear = GL_LINEAR;
constexpr auto rgb = GL_RGB;
constexpr auto red = GL_RED;
constexpr auto rgba = GL_RGBA;
constexpr auto unsigned_byte = GL_UNSIGNED_BYTE;
} // namespace gl

export namespace gf
{
auto& drawElements = glDrawElements;
}
