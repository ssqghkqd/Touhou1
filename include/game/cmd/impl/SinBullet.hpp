#pragma once
#include "entt/entity/registry.hpp"
#include "json.hpp"
#include "vec2.hpp"

namespace th::cmd::impl
{
/*
 * 波与粒的境界
 * 角速度 w = amplitude * sin(frequency * t)
 * 弹幕向周围甩出 segments个分段
 */
struct SinBulletConfig
{
    glm::vec2 position = {0.0f, 0.0f};
    float velocity = 0.0f;
    float angleFrequency = 0.0f;
    float amplitude = 0.0f;
    int segments = 0;
    float spawnInterval = 0.0f;

};

struct SinBulletState
{
    float baseAngle = 0.0f;
    float spawnTimer = 0.0f;
};

struct SinBullet
{
    SinBulletConfig config;
    SinBulletState state;
};

void exec(entt::registry& reg, SinBullet& cmd);
SinBullet parseSinBullet(const nlohmann::json& json);
}