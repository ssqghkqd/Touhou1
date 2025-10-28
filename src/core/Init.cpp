#include "core/Init.hpp"

#include "core/InputSystem.hpp"
#include "core/Window.hpp"
#include "graphics/MeshManager.hpp"
#include "graphics/RenderSystem.hpp"
#include "graphics/ShaderManager.hpp"
#include "graphics/TextureManager.hpp"
#include "resources/AudioManager.hpp"
#include "spdlog/spdlog.h"
#include "utils/Time.hpp"

namespace th::Init
{
void init(entt::registry& reg)
{
    setStatus(reg);
    loadResources(reg);
}

void loadResources(entt::registry& reg)
{
    auto& texture = reg.ctx().get<TextureManager>();
    // 纹理
    texture.loadTexture("player", "textures/player.png");
    texture.loadTexture("hitbox", "textures/hitbox.png");
    texture.loadTexture("xiaoyu", "textures/xiaoyu.png");
    texture.loadTexture("bg1", "textures/bg1.png");
    // 音频
    auto& audio = reg.ctx().get<AudioManager>();
    audio.loadSound("miss", "miss.wav");
    audio.playMusic("th11_09.wav");
}

void setStatus(entt::registry& reg)
{
    // 更新时间
    Time::update();
    spdlog::set_pattern("[%H:%M:%S][%l] - %v");
    // 初始化并存入注册表
    reg.ctx().emplace<Window>();
    // 输入系统不初始化
    reg.ctx().emplace<InputSystem>();
    // mesh不初始化
    reg.ctx().emplace<MeshManager>();
    // 初始化渲染系统
    reg.ctx().emplace<RenderSystem>(reg);
    // shader管理器
    reg.ctx().emplace<ShaderManager>();
    // 纹理系统
    reg.ctx().emplace<TextureManager>();
    // 音频系统
    auto& audio = reg.ctx().emplace<AudioManager>();
    audio.setMasterVolume(1.0f);

}
}