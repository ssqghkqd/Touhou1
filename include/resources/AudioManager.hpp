#pragma once

#include <miniaudio.h>

#include <list>
#include <memory>
#include <string>
#include <unordered_map>

#include "utils/FileManager.hpp"
#include "vec2.hpp"

namespace th
{

class AudioManager
{
  private:
    ma_engine engine{};
    ma_sound music{};  // 音乐专用通道
    std::unordered_map<std::string, ma_sound_group> groups;  // 音效分组
    std::list<std::unique_ptr<ma_sound>> soundPool;  // 音效实例池

    float masterVolume = 1.0f;
    float musicVolume = 1.0f;
    float sfxVolume = 1.0f;
    bool inited = false;

    AudioManager() = default;


  public:
    bool init();
    ~AudioManager();


    static AudioManager& getInstance();

    bool loadSound(const std::string& name, const fs::path& path);


    void playSound(const std::string& name, float volume = 1.0f, const glm::vec2& position = glm::vec2(0.0f), bool loop = false);

    void playMusic(const std::string& path, float volume = 1.0f, bool loop = true);

    void setMasterVolume(float volume);

    void configureSound(ma_sound* sound, float volume, bool loop) const;

};

} // namespace th