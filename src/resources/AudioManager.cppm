// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
// 音频管理器
module;
#include <filesystem>
#include <list>
#include <string>
#include <unordered_map>

export module resources:AudioManager;
import miniaudio;
import spdlog;
import utils;

namespace fs = std::filesystem;

export namespace th
{

class AudioManager
{
  private:
    ma::engine engine_{};
    ma::sound music_{};                                                   // 音乐专用通道
    std::unordered_map<std::string, std::filesystem::path> musicPaths_{}; // 音乐路径表
    std::unordered_map<std::string, ma::sound*> sounds_{};                // 音效
    std::list<ma::sound*> activeSounds_{};

    float masterVolume_ = 0.2f;
    float musicVolume_ = 1.0f;
    float sfxVolume = 1.0f;

  public:
    AudioManager()
    {
        const ma::result result = ma::engineInit(nullptr, &engine_);
        if (result != ma::success)
        {
            spdlog::critical("ma引擎初始化失败:{}", std::string(ma::resultDescription(result)));
            throw std::runtime_error("音频系统初始化失败");
        }
        spdlog::info("音频系统初始化成功");
    }

    ~AudioManager()
    {
        ma::soundUninit(&music_);
        for (auto& [name, sound] : sounds_)
        {
            ma::soundUninit(sound);
        }
        ma::engineUninit(&engine_);
    }

    void loadMusic(const std::string& name, const fs::path& path)
    {
        if (!FileUtils::isExist(path))
        {
            spdlog::critical("音乐加载失败，{}不存在", path.string());
            throw std::runtime_error("音乐加载失败");
        }
        musicPaths_[name] = path;
        spdlog::info("存储音乐{}->{}", name, path.string());
    }

    // 播放音乐（单实例）
    void playMusic(const std::string& name, float volume, bool loop) noexcept
    {
        if (!musicPaths_.contains(name))
        {
            spdlog::error("{} 未加载", name);
            return;
        }

        ma::soundUninit(&music_);
        const auto fullPath = FileUtils::getResourcePath(musicPaths_[name], true).value();

        const auto result = ma::soundInitFromFile(
            &engine_,
            fullPath.c_str(),
            ma::sound_flag_stream | ma::sound_flag_async,
            nullptr,
            nullptr,
            &music_);

        if (result == ma::success)
        {
            ma::soundSetVolume(&music_, volume * musicVolume_ * masterVolume_);
            ma::soundSetLooping(&music_, loop);
            ma::soundStart(&music_);
            return;
        }
        spdlog::error("音乐播放失败 名称{}，原因:{}", name, ma::resultDescription(result));
    }

    /*
     * sounds哈希表里存储的是模板masound对象，几乎静态
     * 播放时将会分配一块内存通过ma提供的复制播放来播放，存入activesound链表 所有权归他所有并且由cleansound定时检查清理
     * 一些踩坑实录：ma没有提供什么一次加载直接播放什么的 搞了半天最后看到了原来是这样实现的
     */
    void loadSound(const std::string& name, const fs::path& path)
    {
        if (sounds_.contains(name))
        {
            spdlog::warn("音频已经加载，名：{}", name);
            return;
        }

        const auto fullPathPossible = FileUtils::getResourcePath(path, true);

        if (!fullPathPossible.has_value())
        {
            spdlog::critical("音效加载失败，文件不存在");
            throw std::runtime_error("音效加载失败");
        }
        const auto& fullPath = fullPathPossible.value();

        const auto templateSound = (ma::sound*)malloc(sizeof(ma::sound));
        if (templateSound == nullptr)
        {
            spdlog::critical("内存不足，分配失败");
            throw std::runtime_error("内存不足");
        }
        const ma::result result = ma::soundInitFromFile(
            &engine_,
            fullPath.c_str(),
            ma::sound_flag_decode,
            nullptr,
            nullptr,
            templateSound);

        if (result != ma::success)
        {
            spdlog::error("加载{} ,失败: {}", path.string(), ma::resultDescription(result));
            free(templateSound);
            throw std::runtime_error("音效加载失败");
        }

        sounds_[name] = templateSound; // 存储模板
    }

    void playSound(const std::string& name, const float volume, bool loop) noexcept
    {
        if (!sounds_.contains(name))
        {
            spdlog::error("{} 未加载", name);
            return;
        }

        /*
         * 【内存管理经验】（亲身经历都是）
         * 1.第一反应当然是智能指针，但实测是不行的：首先 如果你直接使用智能指针，会编译报错 指出无法删除这个masound对象
         *      如果你手动加上删除器 这里有一个问题 智能指针的析构如果再不正确的时间调用，有可能会导致段错误，因为音频驱动还在用这个堆内存
         * 2.那智能指针不能用 只有两个了 new malloc 为什么是malloc？
         * 因为 new会尝试调用构造函数 而masound没有 虽然这其实不会有什么效果 但感觉是是很奇怪的 并且new还会抛出异常 不符合我的理念
         * 最终使用malloc 首先ma就是一个c库他的示例就是这样做的 并且malloc的开销是最小的
         */
        // TODO 可以用对象池 这里malloc可能内存碎片 但目前性能好不用
        const auto sound = (ma::sound*)malloc(sizeof(ma::sound));
        if (sound == nullptr)
        {
            spdlog::error("malloc失败（？）");
            return;
        }
        const ma::result result = ma::soundInitCopy(
            &engine_,
            sounds_[name],
            ma::sound_flag_decode,
            nullptr,
            sound);

        if (result != ma::success)
        {
            free(sound);
            spdlog::error("播放失败 {}: {}", name, ma::resultDescription(result));
            return;
        }

        ma::soundSetVolume(sound, volume * masterVolume_ * sfxVolume);
        ma::soundSetLooping(sound, loop);
        ma::soundStart(sound);
        activeSounds_.push_back(sound);
    }

    void cleanSound()
    {
        /*
         * 将会被每秒调用 注意这里播放完的音频将被清理！不要手动管理
         * 频率不要太高（目前1hz） 一开始我用是vector 现在是链表 但仍然保持低频清理
         */
        for (auto it = activeSounds_.begin(); it != activeSounds_.end();)
        {
            if (!ma::soundIsPlaying(*it))
            {
                ma::soundUninit(*it);
                free(*it);
                it = activeSounds_.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }

    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
    AudioManager(AudioManager&&) noexcept = default;
    AudioManager& operator=(AudioManager&&) noexcept = default;
};

} // namespace th