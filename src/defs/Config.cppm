// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
//
module;

export module defs:Config;
import :Type;

export namespace th::defs
{
constexpr ui32 window_width = 1280;
constexpr ui32 window_height = 960;
constexpr ui32 bg_width = 768;
constexpr ui32 bg_height = 896;
constexpr ui32 bg_offset_x = 64;
constexpr ui32 bg_offset_y = 32;
constexpr ui32 window_scale = 1;

constexpr const char* game_name = "东方宗教之战 ~ religious war";
constexpr const char* engine_name = "ssqghkqd engine2D";
constexpr const char* credits[] = {"程序:ssqghkqd", "美术:盖饭"};

constexpr f32 stat_interval = 1.0f;
constexpr ui32 max_frames_in_flight = 2;
} // namespace th::defs