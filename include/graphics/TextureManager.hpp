#pragma once

#include <unordered_map>
#include <string>
#include <filesystem>
#include "glad.h"

namespace fs = std::filesystem;

namespace th
{
    class TextureManager
    {
    public:
        ~TextureManager();
        GLuint loadTexture(const std::string& textureName, const fs::path& path);
        GLuint getTexture(const std::string &name) const;
        void bind(const std::string &name) const;
        void clear();
        void init();

        // 删除拷贝构造和赋值操作符
        TextureManager(const TextureManager &) = delete;
        TextureManager &operator=(const TextureManager &) = delete;

        // 移动语义也删除（单例不需要）
        TextureManager(TextureManager &&) = delete;
        TextureManager &operator=(TextureManager &&) = delete;

        TextureManager();

    private:
        std::unordered_map<std::string, GLuint> textures; // 纹理缓存
        bool inited = false;
    };
}