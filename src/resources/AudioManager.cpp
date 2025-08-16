#include "resources/AudioManager.hpp"
#include "utils/FileManager.hpp"
#include "core/App.hpp"
#include "SFML/Audio.hpp"
#include <mutex>

namespace th
{
    AudioManager &AudioManager::getInstance()
    {
        static AudioManager instance;
        return instance;
    }

    void AudioManager::init()
    {
        if (inited)
        {
            thLogger::warning("音频系统已初始化");
            return;
        }

        thLogger::info("初始化音频系统");
        // SFML音频系统会自动初始化
    }

    // path默认在sounds下
    bool AudioManager::loadSound(const std::string &name, const fs::path &path)
    {
        auto m_path = FileManager::getResourcePath("sounds/" + path.string());
        std::lock_guard<std::mutex> lock(audioMutex);

        // 检查是否已加载
        if (sounds.find(name) != sounds.end())
        {
            thLogger::warning("音效已加载: " + name);
            return true;
        }

        // 加载音效
        SoundInstance instance;
        if (!instance.buffer.loadFromFile(m_path))
        {
            thLogger::error("无法加载音效: " + m_path.string());
            return false;
        }

        instance.sound = std::make_unique<sf::Sound>(instance.buffer);
        sounds[name] = std::move(instance);

        thLogger::info("音效加载成功: " + name + " (" + m_path.string() + ")");
        return true;
    }

    void AudioManager::playSound(const std::string &name, float volume,
                                 const glm::vec2 &position, bool loop)
    {
        std::lock_guard<std::mutex> lock(audioMutex);

        // 查找音效
        auto it = sounds.find(name);
        if (it == sounds.end())
        {
            thLogger::error("音效未找到: " + name);
            return;
        }

        // 创建新实例（允许多次播放）
        auto newSound = std::make_shared<sf::Sound>(it->second.buffer);

        // 设置参数
        newSound->setVolume(volume * (masterVolume / 100.0f));
        newSound->setLooping(loop);

        // 简单2D定位（左右声道平衡）
        // float balance = glm::clamp(position.x / App::width, -1.0f, 1.0f);
        // newSound->setPosition({balance * 100.0f, 0.0f, 0.0f});

        // 播放
        newSound->play();

        // 添加到活动列表
        activeSounds.push_back(newSound);

        thLogger::info("播放音效: " + name);
    }

    void AudioManager::update()
    {
        // 移除已完成的音效
        auto it = activeSounds.begin();
        while (it != activeSounds.end())
        {
            if ((*it)->getStatus() == sf::Sound::Status::Stopped)
            {
                thLogger::info("清理完成音效");
                it = activeSounds.erase(it); // 移除并获取下一个
            }
            else
            {
                ++it;
            }
        }
    }

    void AudioManager::setMasterVolume(float volume)
    {
        std::lock_guard<std::mutex> lock(audioMutex);
        masterVolume = glm::clamp(volume, 0.0f, 100.0f);

        // 更新所有活动音效
        for (auto &sound : activeSounds)
        {
            float originalVolume = sound->getVolume() / (masterVolume / 100.0f);
            sound->setVolume(originalVolume * (masterVolume / 100.0f));
        }
    }

    void AudioManager::pauseAll()
    {
        std::lock_guard<std::mutex> lock(audioMutex);
        for (auto &sound : activeSounds)
        {
            sound->pause();
        }
    }

    void AudioManager::resumeAll()
    {
        std::lock_guard<std::mutex> lock(audioMutex);
        for (auto &sound : activeSounds)
        {
            sound->play();
        }
    }

    void AudioManager::stopAll()
    {
        std::lock_guard<std::mutex> lock(audioMutex);
        for (auto &sound : activeSounds)
        {
            sound->stop();
        }
        activeSounds.clear();
    }

    AudioManager::~AudioManager()
    {
        stopAll();
    }
} // namespace th
