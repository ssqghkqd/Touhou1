// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
// spdlog包装
module;
#include <spdlog/spdlog.h>
export module spdlog;

export namespace spdlog
{
using spdlog::critical;
using spdlog::debug;
using spdlog::error;
using spdlog::info;
using spdlog::warn;
using spdlog::set_pattern;
using spdlog::set_level;

using level::level_enum;
}
