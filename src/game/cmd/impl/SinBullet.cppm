module;
#include <cmath>
module game.cmd.impl:SinBullet;
import utils;
import entt;
import glm;
import nlohmann_json;

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
    float spawnInterval = 1.0f;
};

struct SinBulletState
{
    float baseAngle = 0.0f;
    float spawnTimer = 0.0f;
};

export struct SinBullet
{
    SinBulletConfig config;
    SinBulletState state;
};

// 原型： 想起「波と粒の境界」（dld五面）
export void exec(entt::registry& reg, SinBullet& cmd)
{
    auto& timer = cmd.state.spawnTimer;
    auto& baseAngle = cmd.state.baseAngle;
    const float A = cmd.config.amplitude; // 振幅
    const float w = cmd.config.angleFrequency; // 角频率 ω
    const float interval = cmd.config.spawnInterval;
    const int seg = cmd.config.segments;
    const auto dt = (float)Time::getDeltaTime();
    const auto t = (float)Time::getTime();

    timer += dt;
    const float angleVel = A * std::sin(w * t);
    baseAngle += angleVel * dt;
    baseAngle = (float)fmod(baseAngle, 2.0 * M_PI);

    if (timer >= interval)
    {
        timer = 0.0f;
        for (int i = 0; i < seg; i++)
        {
            const double angle = baseAngle + i * (M_PI * 2 / seg);
            auto v = glm::vec2(std::cos(angle), std::sin(angle)) * cmd.config.velocity;
            //BulletSys::createBullet(reg, cmd.config.position, v, "bullet_default");
        }
    }
}

export SinBullet parseSinBullet(const nlohmann::json& json)
{
    auto& params = json["params"];
    SinBullet cmd;
    cmd.config.position.x = params.value("posX", 0.0f);
    cmd.config.position.y = params.value("posY", 0.0f);
    cmd.config.segments = params.value("seg", 0);
    cmd.config.amplitude = params.value("amplitude", 0.0f);
    cmd.config.angleFrequency = params.value("w", 0.0f);
    cmd.config.velocity = params.value("v", 0.0f);
    cmd.config.spawnInterval = params.value("interval", 1.0f);
    return cmd;
}
} // namespace th::cmd::impl