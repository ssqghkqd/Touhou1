#pragma once

#include <variant>
#include "game/cmd/impl/SinBullet.hpp"

namespace th::cmd
{
// 请在这里添加更多指令
using CommandVariant = std::variant<
    impl::SinBullet>;
}