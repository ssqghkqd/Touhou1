#include "game/cmd/CmdPlayer.hpp"

#include <algorithm>

#include "entt/entt.hpp"
#include "game/cmd/impl/SinBullet.hpp"
#include "json.hpp"
#include "spdlog/spdlog.h"
#include "utils/Time.hpp"

namespace th::cmd
{

void CmdPlayer::load(const nlohmann::json& json)
{
    cmds.clear();
    for (const auto& item : json["pattern"])
    {
        cmd entry;
        entry.startTime = item["startTime"];
        entry.duration = item.value("duration", -1.0f);
        entry.endTime = entry.duration > 0 ? entry.startTime + entry.duration : -1.0f;
        std::string cmdType = item["cmd"];
        if (cmdType == "sinbullet")
        {
            entry.cmdVariant = CommandVariant{impl::parseSinBullet(item)};
        }
        else
        {
            spdlog::error("未知指令{}", cmdType);
        }
        cmds.push_back(entry);
    }
    std::ranges::sort(cmds, [](const cmd& a, const cmd& b)
                      {
                          return a.startTime < b.startTime;
                      });
}

void CmdPlayer::update(entt::registry& reg)
{
    const float dt = Time::getDeltaTime();
    t += dt;

    /*阶段1 2 检查已经结束的指令 和准备要开始执行的指令*/
    for (auto& item : cmds)
    {
        if (item.isFinished)
        {
            continue;
        }
        if (item.endTime <= t)
        {
            item.isFinished = true;
            item.isActive = false;
            continue;
        }
        if (item.startTime >= t && !item.isActive)
        {
            item.isActive = true;
            activeCmds.push_back(item);
        }
    }

    /*阶段3 执行指令*/
    for (auto& item : activeCmds)
    {
        if (item.isFinished)
        {
            continue;
        }
        std::visit([&reg](auto&& arg)
                   {
                       using T = std::decay_t<decltype(arg)>;
                       if constexpr (std::is_same_v<T, impl::SinBullet>)
                       {
                           impl::exec(reg, arg); // 调用 SinBullet 的 exec 函数
                       }
                   },
                   item.cmdVariant);
    }
}
} // namespace th::cmd