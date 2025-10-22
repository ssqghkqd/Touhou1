
#define MA_ENABLE_VORBIS // 启用libvorbis解码
#define MINIAUDIO_IMPLEMENTATION
#include "resources/AudioManager.hpp"

#include <miniaudio.h>

namespace th
{
bool AudioManager::init()
{
    if (inited)
        return true;

    ma_result result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS)
    {
        thLogger::error("Audio engine init failed: " + std::string(ma_result_description(result)));
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
            NULL, // 音乐不使用音效组
            NULL,
            &music) == MA_SUCCESS)
    {
        ma_sound_set_volume(&music, volume * musicVolume * masterVolume);
        ma_sound_set_looping(&music, loop);
        ma_sound_start(&music);
        return;
    }
    else
    {
        thLogger::error("音乐播放失败");
    }
}

bool AudioManager::loadSound(const std::string& name, const fs::path& path)
{
    if (!inited)
    {
        thLogger::error("音频引擎未初始化");
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
        thLogger::error("Failed to load sound: " + fullPath.string());
        return false;
    }

    soundPool.push_back(std::move(sound));
    return true;
}

void AudioManager::playSound(const std::string& name, const float volume, const glm::vec2& position, bool loop)
{
    if (!inited)
    {
        thLogger::error("音频引擎未初始化");
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
    thLogger::warning("Sound pool exhausted, creating new instance");
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
        thLogger::error("Failed to create sound instance: " +
                        std::string(ma_result_description(result)));
    }
}

void AudioManager::setMasterVolume(const float volume)
{
    masterVolume = glm::clamp(volume, 0.0f, 1.0f);
    ma_engine_set_volume(&engine, masterVolume);
}
AudioManager& AudioManager::getInstance()
{
    static AudioManager instance;
    return instance;
}
AudioManager::~AudioManager()
{
    ma_sound_uninit(&music);
    for (auto& group : groups)
    {
        ma_sound_group_uninit(&group.second);
    }
    ma_engine_uninit(&engine);
    thLogger::info("析构 AudioManager");
}

void AudioManager::configureSound(ma_sound* sound, float volume, bool loop) const
{
    ma_sound_set_volume(sound, volume * sfxVolume * masterVolume);
    ma_sound_set_looping(sound, loop);
}

} // namespace th