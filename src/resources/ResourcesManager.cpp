#include "resources/ResourceManager.hpp"
#include <spdlog/spdlog.h>
#include "graphics/ShaderManager.hpp"

namespace th
{
ResourceManager& ResourceManager::get()
{
    static ResourceManager instance;
    return instance;
}

void ResourceManager::init()
{
    // 加载渲染管线
    ShaderManager::getInstance().loadShader("default", "default.vs", "default.fs");


}
}