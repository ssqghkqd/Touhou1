#pragma once

#include <filesystem>
#include <miniaudio.h>

#include <string>
#include <unordered_map>
#include <vector>

#include "vec2.hpp"

namespace th
{

class AudioManager
{
  private:
    ma_engine engine{};
    ma_sound music{};                                            // 音乐专用通道
    std::unordered_map<std::string, std::filesystem::path> musicPaths{};      // 音乐路径表
    std::unordered_map<std::string, ma_sound*> sounds{}; // 音效
    std::vector<ma_sound*> activeSounds{};

    float masterVolume = 0.2f;
    float musicVolume = 1.0f;
    float sfxVolume = 1.0f;
    bool inited = false;

  public:
    bool init();
    ~AudioManager();
    AudioManager();
    void cleanSound();

    // 禁止拷贝
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    // 允许移动
    AudioManager(AudioManager&&) noexcept = default;
    AudioManager& operator=(AudioManager&&) noexcept = default;

    bool loadSound(const std::string& name, const std::filesystem::path& path);
    void loadMusic(const std::string& name, const std::filesystem::path& path);

    void playSound(const std::string& name, float volume = 1.0f, const glm::vec2& position = glm::vec2(0.0f), bool loop = false);

    void playMusic(const std::string& name, float volume = 1.0f, bool loop = true);
};

} // namespace th