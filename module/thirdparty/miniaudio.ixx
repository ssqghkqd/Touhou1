//
// Created by ss on 2025/11/22.
//
module;
#include <miniaudio/miniaudio.h>

export module miniaudio;

export namespace ma
{
    using engine = ma_engine;
using sound = ma_sound;
using result = ma_result;

auto engine_init = ma_engine_init;
auto result_description = ma_result_description;
auto sound_uninit = ma_sound_uninit;
auto sound_init_from_file = ma_sound_init_from_file;
auto sound_set_volume = ma_sound_set_volume;
auto sound_set_looping = ma_sound_set_looping;
auto sound_start = ma_sound_start;
auto sound_init_copy = ma_sound_init_copy;
auto engine_uninit = ma_engine_uninit;
auto sound_is_playing = ma_sound_is_playing;

constexpr auto SUCCESS = MA_SUCCESS;
constexpr auto SOUND_FLAG_STREAM = MA_SOUND_FLAG_STREAM;
constexpr auto SOUND_FLAG_ASYNC = MA_SOUND_FLAG_ASYNC;
constexpr auto SOUND_FLAG_DECODE = MA_SOUND_FLAG_DECODE;
}