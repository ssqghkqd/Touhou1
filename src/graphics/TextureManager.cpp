#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <filesystem>
#include <graphics/TextureManager.hpp>
#include <utils/FileManager.hpp>

#include "glad.h"
#include "spdlog/spdlog.h"

namespace th
{
TextureManager::~TextureManager()
{
    clear();
}

TextureManager::TextureManager()
{
    init();
}

// path是完整路径（assets下的)
GLuint TextureManager::loadTexture(const std::string& textureName, const fs::path& path)
{
    // 检查是否已加载
    if (textures.contains(textureName))
    {
        return textures[textureName];
    }

    // 获取完整路径
    const std::filesystem::path fullPath = FileManager::getResourcePath(path, true);

    // 加载图像
    int width, height, nrChannels;
    unsigned char* data = stbi_load(fullPath.string().c_str(), &width, &height, &nrChannels, 0);

    if (!data)
    {
        const std::string r = stbi_failure_reason();
        spdlog::error("加载{}失败 原因{}", textureName, r);
        return 0; // 返回无效纹理
    }

    // 创建OpenGL纹理
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 确定格式
    GLenum format = GL_RGB;
    switch (nrChannels)
    {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        default:
            format = GL_RGB;
            break;
    }

    // 上传纹理
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    // 释放图像内存
    stbi_image_free(data);

    // 添加到缓存
    textures[textureName] = textureID;

    spdlog::info("加载纹理{}， 宽{} 高{} 通道数{}", fullPath.string(), width, height, nrChannels);

    return textureID;
}

GLuint TextureManager::getTexture(const std::string& name) const
{
    // 查询map
    auto it = textures.find(name);
    if (it != textures.end())
    {
        return it->second; // ->second代表返回键值 first 返回键名
    }
    return 0; // 无效纹理
}

void TextureManager::bind(const std::string& name) const
{
    auto it = textures.find(name);
    if (it != textures.end())
    {
        glBindTexture(GL_TEXTURE_2D, it->second);
    }
}

void TextureManager::clear()
{
    for (auto& [name, texture] : textures)
    {
        glDeleteTextures(1, &texture);
    }
    textures.clear();
}

void TextureManager::init()
{
    // stbi_set_flip_vertically_on_load(true);
    if (inited)
    {
        spdlog::info("texture manager already inited");
        return;
    }
    inited = true;
}
} // namespace th