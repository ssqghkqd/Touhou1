//
// Created by ss on 2025/11/22.
//
module;
#include <unordered_map>
#include <string>
#include <filesystem>

export module graphics.TextureManager;
import opengl;

namespace fs = std::filesystem;

export namespace th
{
class TextureManager
{
public:
    ~TextureManager();
    gl::uint loadTexture(const std::string& textureName, const fs::path& path);
    gl::uint getTexture(const std::string &name) const;
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
    std::unordered_map<std::string, gl::uint> textures; // 纹理缓存
    bool inited = false;
};
}
