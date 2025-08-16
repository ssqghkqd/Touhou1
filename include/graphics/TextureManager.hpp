#pragma once
#include <stdafx.hpp>
#include <unordered_map>

namespace th
{
    class TextureManager
    {
    public:
        static TextureManager &getInstance();
        ~TextureManager();
        GLuint loadTexture(const std::string &textureName);
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

    private:
        TextureManager() = default;
        std::unordered_map<std::string, GLuint> textures; // 纹理缓存
        bool inited = false;
    };
}