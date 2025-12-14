module;
#include <cmath>
#include <string>
#include <glm/ext.hpp>
#include <entt/entt.hpp>
module game.cmd.impl.SinBullet;
import utils.Time;
import game.system.BulletSys;

namespace th::cmd::impl
{

// 原型： 想起「波と粒の境界」（dld五面）
void exec(entt::registry& reg, SinBullet& cmd)
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
            BulletSys::createBullet(reg, cmd.config.position, v, "bullet_default");
        }
    }
}

SinBullet parseSinBullet(const nlohmann::json& json)
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