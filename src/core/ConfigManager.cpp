#include "core/ConfigManager.hpp"

#include "utils/JsonManager.hpp"

namespace th
{

void ConfigManager::loadBullet(const std::string& jsonName, const std::string& patternName)
{
    auto& j = JsonManager::get(jsonName);
    bulletConfig config;
    config.textureName = j["textureName"].get<std::string>();
    config.size =
}

}