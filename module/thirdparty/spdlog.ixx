//
// Created by ss on 2025/11/22.
//
module;
#include <spdlog/include/spdlog/spdlog.h>

export module spdlog;

export namespace spdlog
{
using spdlog::critical;
using spdlog::debug;
using spdlog::error;
using spdlog::info;
using spdlog::warn;
using spdlog::set_pattern;
} // namespace spdlog
