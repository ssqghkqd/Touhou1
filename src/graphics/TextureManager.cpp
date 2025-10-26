#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <filesystem>
#include <graphics/TextureManager.hpp>
#include <iostream>
#include <utils/FileManager.hpp>

#include "glad.h"
#include "utils/Logger.hpp"

namespace th
{
    TextureManager::~TextureManager()
    {
        clear();
    }

    TextureManager &TextureManager::getInstance()
    {
        static TextureManager instance;
        if (!instance.inited)
            instance.init();
        return instance;
    }

    // 默认所有纹理均在textures下，png格式
    GLuint TextureManager::loadTexture(const std::string &textureName)
    {
        // 检查是否已加载
        if (textures.find(textureName) != textures.end())
        {
            return textures[textureName];
        }

        // 获取完整路径
        std::filesystem::path fullPath = FileManager::getResourcePath("textures/" + textureName + ".png");

        // 加载图像
        int width, height, nrChannels;
        unsigned char *data = stbi_load(fullPath.string().c_str(), &width, &height, &nrChannels, 0);

        if (!data)
        {
            thLogger::error("Failed to load texture: " + fullPath.string());
            const std::string r = stbi_failure_reason();
            thLogger::error("Reason: " + r);
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
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        // 上传纹理
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glActiveTexture(GL_TEXTURE0);
        // 释放图像内存
        stbi_image_free(data);

        // 添加到缓存
        textures[textureName] = textureID;

        std::cout << "Loaded texture: " << fullPath
                  << " (" << width << "x" << height << "), channels: " << nrChannels << "\n";

        return textureID;
    }

    GLuint TextureManager::getTexture(const std::string &name) const
    {
        // 查询map
        auto it = textures.find(name);
        if (it != textures.end())
        {
            return it->second; // ->second代表返回键值 first 返回键名
        }
        return 0; // 无效纹理
    }

    void TextureManager::bind(const std::string &name) const
    {
        auto it = textures.find(name);
        if (it != textures.end())
        {
            glBindTexture(GL_TEXTURE_2D, it->second);
        }
    }

    void TextureManager::clear()
    {
        for (auto &[name, texture] : textures)
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
            thLogger::warning("texturemanager已初始化");
            return;
        }
        inited = true;
    }
}