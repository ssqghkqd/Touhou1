module;
#include <cstdlib>
#include <filesystem>
#include <string>
#include <glm/ext.hpp>
module resources.AudioManager;
import spdlog;
import utils.FileManager;

namespace fs = std::filesystem;

namespace th
{
bool AudioManager::init()
{
    if (inited)
        return true;

    const ma::result result = ma::engine_init(nullptr, &engine);
    if (result != ma::success)
    {
        spdlog::error("ma_engine_init failed {}", std::string(ma::result_description(result)));
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

    ma::sound_uninit(&music);
    const auto fullPath = FileManager::getResourcePath(musicPaths[name], true);

    // 数星星
    const auto result = ma::sound_init_from_file(
        &engine,
        fullPath.c_str(),
        ma::sound_flag_stream | ma::sound_flag_async,
        nullptr,
        nullptr,
        &music);

    if (result == ma::success)
    {
        ma::sound_set_volume(&music, volume * musicVolume * masterVolume);
        ma::sound_set_looping(&music, loop);
        ma::sound_start(&music);
        return;
    }
    spdlog::error("音乐播放失败 名称{}，原因:{}", name, ma::result_description(result));
}

/*
 * sounds哈希表里存储的是模板masound对象，几乎静态
 * 播放时将会分配一块内存通过ma提供的复制播放来播放，存入activesound链表 所有权归他所有并且由cleansound定时检查清理
 * 一些踩坑实录：ma没有提供什么一次加载直接播放什么的 搞了半天最后看到了原来是这样实现的
 */

bool AudioManager::loadSound(const std::string& name, const fs::path& path)
{
    if (!inited)
    {
        spdlog::error("音频系统未初始化");
        return false;
    }
    if (sounds.contains(name))
    {
        spdlog::warn("音频已经加载，名：{}", name);
        return false;
    }

    const auto fullPath = FileManager::getResourcePath(path.string(), true);

    auto templateSound = (ma::sound*)malloc(sizeof(ma::sound));
    if (templateSound == nullptr)
    {
        spdlog::error("malloc templateSound failed");
        return false;
    }
    const ma::result result = ma::sound_init_from_file(
        &engine,
        fullPath.c_str(),
        ma::sound_flag_decode,
        nullptr,
        nullptr,
        templateSound);

    if (result == ma::success)
    {
        sounds[name] = templateSound; // 存储模板
        return true;
    }
    spdlog::error("加载{} ,失败: {}", path.string(), ma::result_description(result));
    free(templateSound);
    return false;
}

void AudioManager::playSound(const std::string& name, const float volume, [[maybe_unused]] const glm::vec2& position, bool loop)
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
     * 1.第一反应是使用智能指针 但这里不能使用
     *  如果你直接使用 会导致编译失败
     *  如果你套用自定义删除器 你需要尤其注意move出去移动所有权 否则自动析构 音频驱动将会段错误
     * 2.为什么不用new？ 因为new会尝试调用构造函数 但masound没有，并且还有异常开销
     * 提示：成功路径不需要释放 因为有清理函数检查 失败路径一定要free（malloc成功之后）
     */
    // TODO 可以用对象池 这里malloc可能内存碎片 但目前性能好不用
    // ReSharper disable once CppCStyleCast
    auto sound = (ma::sound*)malloc(sizeof(ma::sound));
    if (sound == nullptr)
    {
        spdlog::error("malloc failed");
        return;
    }
    const ma::result result = ma::sound_init_copy(
        &engine,
        sounds[name],
        ma::sound_flag_decode,
        nullptr,
        sound);

    if (result != ma::success)
    {
        free(sound);
        spdlog::error("播放失败 {}: {}", name, ma::result_description(result));
        return;
    }

    ma::sound_set_volume(sound, volume * masterVolume * sfxVolume);
    ma::sound_set_looping(sound, loop);
    ma::sound_start(sound);
    activeSounds.push_back(sound);
}

AudioManager::~AudioManager()
{
    ma::sound_uninit(&music);
    for (auto& [name, sound] : sounds)
    {
        ma::sound_uninit(sound);
    }
    ma::engine_uninit(&engine);
}

AudioManager::AudioManager()
{
    init();
}

void AudioManager::cleanSound()
{
    /*
     * 将会被每秒调用 注意这里播放完的音频将被清理！不要手动管理
     * 频率不要太高（目前1hz） 一开始我用是vector 现在是链表 但仍然保持低频清理
     */
    for (auto it = activeSounds.begin(); it != activeSounds.end();)
    {
        if (!ma::sound_is_playing(*it))
        {
            ma::sound_uninit(*it);
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