#define MINIAUDIO_IMPLEMENTATION
#include "resources/AudioManager.hpp"

#include <miniaudio.h>

namespace th
{
bool AudioManager::init()
{
    ma_result result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS)
    {
        thLogger::error("miniaudio 引擎初始化失败");
        return false;
    }
    inited = true;
    thLogger::info("miniaudio 引擎初始化成功");
    return true;
}
bool AudioManager::loadSound(const std::string& name, const fs::path& path)
{
    if (!inited)
    {
        thLogger::error("音频引擎未初始化");
        return false;
    }

    auto fullPath = FileManager::getResourcePath("sounds/" + path.string());

    if (sounds.find(name) != sounds.end())
    {
        thLogger::warning("音效已加载: " + name);
        return true;
    }

    auto sound = std::make_unique<ma_sound>();
    ma_result result = ma_sound_init_from_file(&engine, fullPath.string().c_str(), MA_SOUND_FLAG_DECODE, NULL, NULL, sound.get());

    if (result != MA_SUCCESS)
    {
        thLogger::error("加载音效失败: " + fullPath.string());
        thLogger::error("miniaudio 错误: " + std::string(ma_result_description(result)));
        return false;
    }

    sounds[name] = std::move(sound);
    thLogger::info("加载音效成功: " + fullPath.string());
    return true;
}
void AudioManager::playSound(const std::string& name, const float volume, const glm::vec2& position, bool loop)
{
    if (!inited)
    {
        thLogger::error("音频引擎未初始化");
        return;}

    auto it = sounds.find(name);
    if (it == sounds.end())
{
        thLogger::error("音效未找到: " + name);
        return;
    }

    ma_sound_set_volume(it->second.get(), volume * masterVolume);

    // 2D 声像定位 (position.x 在 -1.0 到 1.0 之间)
    // float pan = glm::clamp(position.x / 10.0f, -1.0f, 1.0f); // 调整除数以控制声像范围
    // ma_sound_set_pan(it->second.get(), pan);

    ma_sound_set_looping(it->second.get(), loop);
    ma_sound_start(it->second.get());
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
    // 先停止所有声音
    ma_engine_stop(&engine);  // 核心API：停止引擎所有声音
    for (auto& [name, sound] : sounds) {
        ma_sound_uninit(sound.get());
    }
    sounds.clear(); // 自动释放所有 ma_sound 对象
    ma_engine_uninit(&engine);
    thLogger::info("析构 AudioManager");
}

} // namespace th