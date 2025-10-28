
#define MA_ENABLE_VORBIS // 启用libvorbis解码
#define MINIAUDIO_IMPLEMENTATION
#include "resources/AudioManager.hpp"

#include <miniaudio.h>

#include "ext.hpp"
#include "spdlog/spdlog.h"

namespace th
{
bool AudioManager::init()
{
    if (inited)
        return true;

    ma_result result = ma_engine_init(nullptr, &engine);
    if (result != MA_SUCCESS)
    {
        spdlog::error("ma_engine_init failed {}", std::string(ma_result_description(result)));
        return false;
    }

    // 创建默认音效组
    ma_sound_group_init(&engine, 0, NULL, &groups["default"]);
    inited = true;
    return true;
}

// 播放音乐（单实例）
void AudioManager::playMusic(const std::string& path, float volume, bool loop)
{
    if (!inited)
        return;

    ma_sound_uninit(&music);
    auto fullPath = FileManager::getResourcePath("music/" + path);

    if (ma_sound_init_from_file(
            &engine,
            fullPath.c_str(),
            MA_SOUND_FLAG_DECODE| MA_SOUND_FLAG_NO_SPATIALIZATION,
            nullptr, // 音乐不使用音效组
            nullptr,
            &music) == MA_SUCCESS)
    {
        ma_sound_set_volume(&music, volume * musicVolume * masterVolume);
        ma_sound_set_looping(&music, loop);
        ma_sound_start(&music);
        return;
    }
    else
    {
        spdlog::error("音乐播放失败");
    }
}

bool AudioManager::loadSound(const std::string& name, const fs::path& path)
{
    if (!inited)
    {
        spdlog::error("音频系统未初始化");
        return false;
    }

    const auto fullPath = FileManager::getResourcePath("sounds/" + path.string());
    auto sound = std::make_unique<ma_sound>();

    const ma_result result = ma_sound_init_from_file(
        &engine,
        fullPath.c_str(),
        MA_SOUND_FLAG_DECODE | MA_SOUND_FLAG_ASYNC,
        &groups["default"], // 绑定到默认音效组
        NULL,
        sound.get());

    if (result != MA_SUCCESS)
    {
        spdlog::error("加载失败:{}, 原因{}", fullPath.string(), std::string(ma_result_description(result)));
        return false;
    }

    soundPool.push_back(std::move(sound));
    return true;
}

void AudioManager::playSound(const std::string& name, const float volume, const glm::vec2& position, bool loop)
{
    if (!inited)
    {
        spdlog::error("音频系统未初始化");
        return;
    }

    for (auto& sound : soundPool)
    {
        if (!ma_sound_is_playing(sound.get()))
        {
            ma_sound_set_volume(sound.get(), volume * sfxVolume * masterVolume);
            ma_sound_set_looping(sound.get(), loop);
            //const float pan = glm::clamp(position.x / 10.0f, -1.0f, 1.0f); // 调整除数以控制声像范围
            //ma_sound_set_pan(sound.get(), pan);
            ma_sound_start(sound.get());
            return;
        }
    }

    // 没有可用实例时创建新实例
    auto newSound = std::make_unique<ma_sound>();
    ma_result result = ma_sound_init_copy(
        &engine,
        soundPool.front().get(), // 复制第一个实例的配置
        MA_SOUND_FLAG_DECODE,    // 保持解码标志
        &groups["default"],      // 使用默认音效组
        newSound.get());

    if (result == MA_SUCCESS)
    {
        configureSound(newSound.get(), volume, loop);
        //const float pan = glm::clamp(position.x / 10.0f, -1.0f, 1.0f); // 调整除数以控制声像范围
        //ma_sound_set_pan(newSound.get(), pan);
        ma_sound_start(newSound.get());
        soundPool.push_back(std::move(newSound));
    }
    else
    {
        spdlog::error("创建新实例失败{}", std::string(ma_result_description(result)));
    }
}

void AudioManager::setMasterVolume(const float volume)
{
    masterVolume = glm::clamp(volume, 0.0f, 1.0f);
    ma_engine_set_volume(&engine, masterVolume);
}
AudioManager::~AudioManager()
{
    ma_sound_uninit(&music);
    for (auto& group : groups)
    {
        ma_sound_group_uninit(&group.second);
    }
    ma_engine_uninit(&engine);
}

 AudioManager::AudioManager()
{
    init();
}


void AudioManager::configureSound(ma_sound* sound, float volume, bool loop) const
{
    ma_sound_set_volume(sound, volume * sfxVolume * masterVolume);
    ma_sound_set_looping(sound, loop);
}

} // namespace th