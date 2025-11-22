module;
#include <string>

module core.ConfigManager;
import utils.JsonManager;

namespace th
{

void ConfigManager::loadBullet(const std::string& jsonName, const std::string& patternName)
{
    auto& j = JsonManager::get(jsonName)[patternName];
    bulletConfig config;
    config.textureName = j["textureName"].get<std::string>();
    config.size = {j["width"], j["height"]};
    config.radius = j["collisionRadius"];
    m_BulletConfig[patternName] = config;
}

bulletConfig ConfigManager::getBulletConfig(const std::string& patternName)
{
    return m_BulletConfig[patternName];
}

}