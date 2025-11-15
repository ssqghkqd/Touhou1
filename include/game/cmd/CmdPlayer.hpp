#pragma once
#include "entt/entity/registry.hpp"
#include "json.hpp"
#include "game/cmd/CmdVariant.hpp"

namespace th::cmd
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
    std::vector<cmd> cmds;
    std::vector<cmd> activeCmds;
    float t = 0.0f;
};
} // namespace th::cmd