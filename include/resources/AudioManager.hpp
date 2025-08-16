#pragma once

#include "stdafx.hpp"
#include "SFML/Audio.hpp"
#include <mutex>
#include <filesystem>

namespace fs = std::filesystem;

namespace th
{
    class AudioManager
    {
    public:
        static AudioManager &getInstance();

        // 初始化音频系统
        void init();

        // 加载音效
        bool loadSound(const std::string &name, const fs::path &path);

        // 播放音效
        void playSound(const std::string &name, float volume = 100.0f,
                       const glm::vec2 &position = {0, 0}, bool loop = false);

        // 设置全局音量
        void setMasterVolume(float volume);

        // 暂停所有音效
        void pauseAll();

        // 恢复所有音效
        void resumeAll();

        // 停止所有音效
        void stopAll();

        // 更新 清理已经播放完的声音
        void update();

    private:
        AudioManager() = default;
        ~AudioManager();

        struct SoundInstance
        {
            std::unique_ptr<sf::Sound> sound;
            sf::SoundBuffer buffer;
        };

        std::unordered_map<std::string, SoundInstance> sounds;
        std::vector<std::shared_ptr<sf::Sound>> activeSounds;

        float masterVolume = 100.0f;
        std::mutex audioMutex;
        bool inited = false;
    };
} // namespace th
