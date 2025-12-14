//
// Created by ss on 2025/11/22.
//
module;
#include <vector>
#include <entt/entt.hpp>
export module game.cmd.CmdPlayer;
import nlohmann_json;
import game.cmd.CmdVariant;

export namespace th::cmd
{
struct cmd
{
    float startTime = 0.0f;
    float duration = 0.0f; // -1.0f代表无限持续
    float endTime = 0.0f;
    bool isActive = false;
    bool isFinished = false;
    CommandVariant cmdVariant;
};

class CmdPlayer
{
public:
    void load(const nlohmann::json& json);
    void update(entt::registry& reg);
private:
    std::vector<cmd> m_cmds;
    std::vector<cmd> m_activeCmds;
    float t = 0.0f;
};
} // namespace th::cmd
