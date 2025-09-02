#pragma once

#include <memory>
#include <string>
#include <miniaudio.h>
#include <unordered_map>

#include "utils/FileManager.hpp"

namespace th
{

class AudioManager
{
  private:
    ma_engine engine{};
    std::unordered_map<std::string, std::unique_ptr<ma_sound>> sounds;
    float masterVolume = 1.0f;
    bool inited = false;

    AudioManager() = default;


  public:
    bool init();
    ~AudioManager();


    static AudioManager& getInstance();

    bool loadSound(const std::string& name, const fs::path& path);


    void playSound(const std::string& name, float volume = 1.0f, const glm::vec2& position = glm::vec2(0.0f), bool loop = false);

    void setMasterVolume(float volume);

};

} // namespace th