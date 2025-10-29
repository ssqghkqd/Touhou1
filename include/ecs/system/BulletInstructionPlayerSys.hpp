#pragma once
#include <string>
#include <vector>
#include <entt/entt.hpp>
#include "json.hpp"

namespace th::BulletInstructionPlayerSys
{
struct Instruction
{
    std::string cmd;
    float time;
    std::vector<float> params;
};

void load(const nlohmann::json& j);

void update(entt::registry& reg, float dt);
void executeInstr(entt::registry& reg, const Instruction& inst);
}