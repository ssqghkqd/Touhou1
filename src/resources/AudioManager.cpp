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
        MA_SOUND_FLAG_STREAM | MA_SOUND_FLAG_ASYNC,
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
    if (templateSound == nullptr)
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

    /*
     * 【内存管理经验】（亲身经历都是）
     * 智能指针在这里是不好的实现
     * 直接使用智能指针：编译错误 使用自定义删除器：你需要尤其注意用move移动所有权，
     *   否则自动析构释放音频内存会直接导致内核音频驱动段错误，并且智能指针会尝试调用构造析构函数 这里是无效的浪费的开销
     * new和malloc在这里等效 但对于一个没有构造函数的c结构体 并且是c库 malloc更合适
     * 成功路径不需要释放 因为有清理函数每帧检查
     */
    auto* sound = (ma_sound*)malloc(sizeof(ma_sound));
    if (sound == nullptr)
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
        ma_sound_set_volume(sound, volume * masterVolume * sfxVolume);
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
    /*
     * 将会被每帧调用 注意这里播放完的音频将被清理！不要手动管理
     */
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