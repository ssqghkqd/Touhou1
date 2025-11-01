#pragma once
#include <string>
#include <vector>
#include <entt/entt.hpp>
#include "json.hpp"

namespace th::BulletInstructionPlayerSys
{
struct Instruction
{
    float time;
    std::string cmd;
    std::vector<float> params;
};

void load(const nlohmann::json& j);

void update(entt::registry& reg, float dt);
void executeInstr(entt::registry& reg, const Instruction& inst);
}