#include "game/cmd/impl/SinBullet.hpp"

#include "game/system/BulletSystem.hpp"
#include "spdlog/spdlog.h"
#include "utils/Time.hpp"

namespace th::cmd::impl
{

void exec(entt::registry& reg, SinBullet& cmd)
{
    auto& timer = cmd.state.spawnTimer;
    auto& baseAngle = cmd.state.baseAngle;
    const float A = cmd.config.amplitude; // 振幅
    const float w = cmd.config.angleFrequency; // 角频率 ω
    const float interval = cmd.config.spawnInterval;
    const int seg = cmd.config.segments;
    const float dt = Time::getDeltaTime();
    const float t = Time::getTime();

    timer += dt;
    const float angleVel = A * std::sin(w * t); // 角速度是正弦变化
    baseAngle += angleVel * dt;
    baseAngle = fmod(baseAngle, 2.0 * M_PI);

    if (timer >= interval)
    {
        timer = 0.0f;
        for (int i = 0; i < seg; i++)
        {
            const double angle = baseAngle + i * (M_PI * 2 / seg);
            auto v = glm::vec2(std::cos(angle), std::sin(angle)) * cmd.config.velocity;
            BulletSystem::createBullet(reg, cmd.config.position, v, "xiaoyu");
        }
    }
}

SinBullet parseSinBullet(const nlohmann::json& json)
{
    auto& params = json["params"];
    SinBullet cmd;
    cmd.config.position.x = params.value("posX", 400.0f);
    cmd.config.position.y = params.value("posY", 400.0f);
    cmd.config.segments = params.value("seg", 8);
    cmd.config.amplitude = params.value("amplitude", 1.0f);
    cmd.config.angleFrequency = params.value("w", 1.0f);
    cmd.config.velocity = params.value("v", 1.0f);
    cmd.config.spawnInterval = params.value("interval", 1.0f);
    return cmd;
}
} // namespace th::cmd::impl