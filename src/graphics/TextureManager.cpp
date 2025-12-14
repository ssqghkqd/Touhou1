module;
#include <string>
#include <filesystem>
module graphics.TextureManager;
import utils.FileManager;
import stb_image;
import spdlog;

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
gl::uint TextureManager::loadTexture(const std::string& textureName, const fs::path& path)
{
    if (textures.contains(textureName))
    {
        return textures[textureName];
    }

    const std::filesystem::path fullPath = FileManager::getResourcePath(path, true);

    int width, height, nrChannels;
    unsigned char* data = stbi::load(fullPath.string().c_str(), &width, &height, &nrChannels, 0);

    if (!data)
    {
        const char* r = stbi::failure_reason();
        spdlog::error("加载{}失败 原因{}", textureName, r);
        return 0;
    }

    gl::glenum format = gl::rgb;
    switch (nrChannels)
    {
        case 1:
            format = gl::red;
            break;
        case 3:
            format = gl::rgb;
            break;
        case 4:
            format = gl::rgba;
            break;
        default:
            stbi::image_free(data);
            spdlog::error("加载{}失败，原因不支持{}通道", fullPath.string(), nrChannels);
            return 0;
    }

    gl::uint textureID;
    gl::genTextures(1, &textureID);
    gl::bindTexture(gl::texture_2d, textureID);

    gl::texParameteri(gl::texture_2d, gl::texture_wrap_s, gl::repeat);
    gl::texParameteri(gl::texture_2d, gl::texture_warp_t, gl::repeat);
    gl::texParameteri(gl::texture_2d, gl::texture_min_filter, gl::linear_mimap_linear);
    gl::texParameteri(gl::texture_2d, gl::texture_mag_filter, gl::linear);

    gl::texImage2D(gl::texture_2d, 0, format, width, height, 0, format, gl::unsigned_byte, data);
    gl::generateMipmap(gl::texture_2d);
    gl::activeTexture(gl::texture0);

    stbi::image_free(data);

    textures[textureName] = textureID;

    spdlog::info("加载纹理{}， 宽{} 高{} 通道数{}", fullPath.string(), width, height, nrChannels);

    return textureID;
}

gl::uint TextureManager::getTexture(const std::string& name) const
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
        gl::bindTexture(gl::texture_2d, it->second);
    }
}

void TextureManager::clear()
{
    for (auto& [name, texture] : textures)
    {
        gl::deleteTextures(1, &texture);
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