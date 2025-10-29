#include "ecs/system/BulletInstructionPlayerSys.hpp"

#include <algorithm>

#include "ecs/comp/SpriteComp.hpp"
#include "ecs/system/BulletSystem.hpp"
#include "json.hpp"
#include "spdlog/spdlog.h"

using json = nlohmann::json;

namespace th::BulletInstructionPlayerSys
{
static std::vector<Instruction> instructions = {};
static float m_time = 0.0f; // 当前时间
static int next_index = 0;
static entt::entity bullet = entt::null; // 当前正在操作的子弹s
static bool inited = false;

/*切换正在使用的json文件*/
void load(const json& j)
{
    instructions.clear();
    for (const auto& item : j["pattern"])
    {
        Instruction inst;
        inst.cmd = item["cmd"].get<std::string>();
        inst.time = item["time"].get<float>();
        for (const auto& params_value : item["params"])
        {
            inst.params.push_back(params_value.get<float>());
        }

        instructions.push_back(inst);
    }

    std::sort(instructions.begin(), instructions.end(), [](const Instruction& a, const Instruction& b)
              {
                  return a.time < b.time;
              });
}

void update(entt::registry& reg, const float dt)
{
    if (!inited)
    {
        m_time = 0.0f;
        inited = true;
    }
    m_time += dt;

    while (next_index < instructions.size())
    {
        const auto& inst = instructions[next_index];
        if (m_time >= inst.time)
        {
            executeInstr(reg, inst);
            next_index++;
        }
        break;
    }
}

void executeInstr(entt::registry& reg, const Instruction& inst)
{
    spdlog::info(inst.cmd);
    /* params: 初始X 初始Y 速度X 速度Y*/
    if (inst.cmd == "shot")
    {
        bullet = BulletSystem::createBullet(reg,
                                   {inst.params[0], inst.params[1]},
                                   {inst.params[2], inst.params[3]});
        spdlog::info("发射子弹，位置({},{}),速度({},{})", inst.params[0], inst.params[1], inst.params[2], inst.params[3]);
    }
    else if (inst.cmd == "v")
    {
        auto& sc = reg.get<SpriteComp>(bullet);
        sc.velocity = {inst.params[0], inst.params[1]};
        spdlog::info("设置速度({},{})", inst.params[0], inst.params[1]);
    }
}
} // namespace th::BulletInstructionPlayerSys