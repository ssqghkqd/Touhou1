//
// Created by ss on 2025/11/22.
//
module;
#include <variant>

export module game.cmd.CmdVariant;
import game.cmd.impl.SinBullet;

export namespace th::cmd
{
// 请在这里添加更多指令
using CommandVariant = std::variant<
    impl::SinBullet>;
}
