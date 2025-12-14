module;
#include <variant>
#include <string>
#include <entt/entt.hpp>
module game.cmd.CmdPlayer;
import spdlog;
import utils.Time;
import game.cmd.impl.SinBullet;


namespace th::cmd
{

void CmdPlayer::load(const nlohmann::json& json)
{
    m_cmds.clear();
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
            continue;
        }
        m_cmds.push_back(entry);
        spdlog::info("存储指令， 指令名{}, 开始时间{} 结束时间{} 持续{}s",
                     cmdType,
                     entry.startTime,
                     entry.endTime,
                     entry.duration);
    }
}

void CmdPlayer::update(entt::registry& reg)
{
    const float dt = (float)Time::getDeltaTime();
    t += dt;

    for (auto& item : m_cmds)
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
        if (item.startTime < t && !item.isActive)
        {
            item.isActive = true;
        }
        if (item.isActive)
        {
            std::visit([&reg](auto&& arg)
                       {
                           using T = std::decay_t<decltype(arg)>;
                           if constexpr (std::is_same_v<T, impl::SinBullet>)
                           {
                               impl::exec(reg, arg);
                           }
                       },
                       item.cmdVariant);
        }
    }
}
} // namespace th::cmd