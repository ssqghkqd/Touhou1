#pragma once

#include <string>
#include <unordered_map>

#include "vec2.hpp"

namespace th
{
struct bulletConfig
{
    // rc
    std::string textureName;
    glm::vec2 size = glm::vec2(0.0f);
    //cc
    float radius = 0.0f;
};

struct playerConfig
{
    float slowSpeed = 0.0f;
    float speed = 0.0f;
    std::string textureName;
    glm::vec2 size = glm::vec2(0.0f);
    float radius = 0.0f;
};

class ConfigManager
{
    public:
    void loadBullet(const std::string& jsonName, const std::string& patternName);
private:
    std::unordered_map<std::string, bulletConfig> m_BulletConfig;
    playerConfig m_PlayerConfig;
};
}