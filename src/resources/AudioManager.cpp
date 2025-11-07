#define MINIAUDIO_IMPLEMENTATION
#include "resources/AudioManager.hpp"

#include <miniaudio.h>

#include "ext.hpp"
#include "spdlog/spdlog.h"
#include "utils/FileManager.hpp"

namespace fs = std::filesystem;

namespace th
{
bool AudioManager::init()
{
    if (inited)
        return true;

    const ma_result result = ma_engine_init(nullptr, &engine);
    if (result != MA_SUCCESS)
    {
        spdlog::error("ma_engine_init failed {}", std::string(ma_result_description(result)));
        return false;
    }

    inited = true;
    return true;
}

void AudioManager::loadMusic(const std::string& name, const fs::path& path)
{
    if (!inited)
    {
        spdlog::error("音频系统未初始化");
        return;
    }
    musicPaths[name] = path;
    spdlog::info("存储音乐{}->{}", name, path.string());
}

// 播放音乐（单实例）
void AudioManager::playMusic(const std::string& name, float volume, bool loop)
{
    if (!inited)
    {
        spdlog::warn("音频管理器未初始化");
        return;
    }
    if (!musicPaths.contains(name))
    {
        spdlog::error("{} 未加载", name);
        return;
    }

    ma_sound_uninit(&music);
    const auto fullPath = FileManager::getResourcePath(musicPaths[name], true);

    // 数星星
    const auto result = ma_sound_init_from_file(
        &engine,
        fullPath.c_str(),
        MA_SOUND_FLAG_DECODE,
        nullptr,
        nullptr,
        &music);

    if (result == MA_SUCCESS)
    {
        ma_sound_set_volume(&music, volume * musicVolume * masterVolume);
        ma_sound_set_looping(&music, loop);
        ma_sound_start(&music);
        return;
    }
    spdlog::error("音乐播放失败 名称{}，原因:{}", name, ma_result_description(result));
}

bool AudioManager::loadSound(const std::string& name, const fs::path& path)
{
    if (!inited)
    {
        spdlog::error("音频系统未初始化");
        return false;
    }

    const auto fullPath = FileManager::getResourcePath(path.string(), true);

    auto* templateSound = (ma_sound*)malloc(sizeof(ma_sound));
    if (!templateSound)
    {
        spdlog::error("malloc templateSound failed");
        return false;
    }
    const ma_result result = ma_sound_init_from_file(
        &engine,
        fullPath.c_str(),
        MA_SOUND_FLAG_DECODE,
        nullptr,
        nullptr,
        templateSound);

    if (result == MA_SUCCESS)
    {
        sounds[name] = templateSound; // 存储模板
        return true;
    }
    spdlog::error("加载{} ,失败: {}", path.string(), ma_result_description(result));
    free(templateSound);
    return false;
}

void AudioManager::playSound(const std::string& name, const float volume, const glm::vec2& position, bool loop)
{
    if (!inited)
    {
        spdlog::error("音频系统未初始化");
        return;
    }
    if (!sounds.contains(name))
    {
        spdlog::error("{} 未加载", name);
        return;
    }

    // 复制一个实例
    /*这里用malloc的原因是智能指针会编译报错 new和malloc在这里都一样 malloc看起来风格更好*/
    auto* sound = (ma_sound*)malloc(sizeof(ma_sound));
    if (!sound)
    {
        spdlog::error("malloc failed");
        return;
    }
    const ma_result result = ma_sound_init_copy(
        &engine,
        sounds[name],
        MA_SOUND_FLAG_DECODE,
        nullptr,
        sound);

    if (result == MA_SUCCESS)
    {
        ma_sound_set_volume(sound, volume);
        ma_sound_set_looping(sound, loop);
        ma_sound_start(sound);
        activeSounds.push_back(sound);
        return;
    }
    free(sound);
    spdlog::error("播放失败 {}: {}", name, ma_result_description(result));
}

AudioManager::~AudioManager()
{
    ma_sound_uninit(&music);
    for (auto& [name, sound] : sounds)
    {
        ma_sound_uninit(sound);
    }
    ma_engine_uninit(&engine);
}

AudioManager::AudioManager()
{
    init();
}

void AudioManager::cleanSound()
{
    for (auto it = activeSounds.begin(); it != activeSounds.end();)
    {
        if (!ma_sound_is_playing(*it))
        {
            ma_sound_uninit(*it);
            free(*it);
            it = activeSounds.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
} // namespace th