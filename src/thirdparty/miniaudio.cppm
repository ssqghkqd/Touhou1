// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
// ma包装
module;
#include <miniaudio/miniaudio.h>
export module miniaudio;

export namespace ma
{
using engine = ma_engine;
using sound = ma_sound;
using result = ma_result;

auto& engineInit = ma_engine_init;
auto& resultDescription = ma_result_description;
auto& soundUninit = ma_sound_uninit;
auto& soundInitFromFile = ma_sound_init_from_file;
auto& soundSetVolume = ma_sound_set_volume;
auto& soundSetLooping = ma_sound_set_looping;
auto& soundStart = ma_sound_start;
auto& soundInitCopy = ma_sound_init_copy;
auto& engineUninit = ma_engine_uninit;
auto& soundIsPlaying = ma_sound_is_playing;

constexpr auto success = MA_SUCCESS;
constexpr auto sound_flag_stream = MA_SOUND_FLAG_STREAM;
constexpr auto sound_flag_async = MA_SOUND_FLAG_ASYNC;
constexpr auto sound_flag_decode = MA_SOUND_FLAG_DECODE;
}
