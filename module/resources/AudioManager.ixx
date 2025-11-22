//
// Created by ss on 2025/11/22.
//
module;
#include <string>
#include <list>
#include <unordered_map>
#include <filesystem>
#include <glm/ext.hpp>
export module resources.AudioManager;
import miniaudio;

export namespace th
{

class AudioManager
{
private:
    ma::engine engine{};
    ma::sound music{};                                            // 音乐专用通道
    std::unordered_map<std::string, std::filesystem::path> musicPaths{};      // 音乐路径表
    std::unordered_map<std::string, ma::sound*> sounds{}; // 音效
    std::list<ma::sound*> activeSounds{};

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