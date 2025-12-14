module;
#include <entt/entt.hpp>

module core.Init;

import spdlog;
import utils.Time;
import utils.JsonManager;
import core.ConfigManager;
import graphics.TextureManager;
import resources.AudioManager;
import core.Window;
import core.InputSystem;
import graphics.MeshManager;
import graphics.ShaderManager;
import graphics.RenderSystem;
import game.cmd.CmdPlayer;
import game.system.PlayerSys;

namespace th::Init
{
void init(entt::registry& reg)
{
    loadCore(reg);
    loadResources(reg);
    gameStatusSet(reg);
}

void loadResources(entt::registry& reg)
{
    auto& texture = reg.ctx().get<TextureManager>();
    auto& audio = reg.ctx().get<AudioManager>();
    auto& cmdPlayer = reg.ctx().get<cmd::CmdPlayer>();
    auto& cfgM = reg.ctx().get<ConfigManager>();
    // 纹理
    texture.loadTexture("player", "textures/player.png");
    texture.loadTexture("hitbox", "textures/hitbox.png");
    texture.loadTexture("xiaoyu", "textures/xiaoyu.png");
    texture.loadTexture("bg1", "textures/bg1.png");
    texture.loadTexture("enemy", "textures/enemy.png");
    // 音频

    audio.loadSound("miss", "sounds/miss.wav");
    audio.loadSound("enemy_shot", "sounds/enemy_shot.wav");
    audio.loadSound("enemy_death", "sounds/enemy_death.wav");
    audio.loadMusic("satori", "music/th11_09.mp3");

    JsonManager::load("json/config/player.json", "config.player");
    JsonManager::load("json/stage/stage1.json", "stage1");
    JsonManager::load("json/config/bullet.json", "config.bullet");

    auto& j = JsonManager::get("stage1");

    cmdPlayer.load(j);
    cfgM.loadBullet("config.bullet", "bullet_default");
}

void loadCore(entt::registry& reg)
{
    // 更新时间
    Time::init();
    spdlog::set_pattern("[%H:%M:%S][%l] - %v");
    spdlog::info("游戏开始初始化");
    // 初始化并存入注册表
    reg.ctx().emplace<Window>();
    // 输入系统不初始化
    reg.ctx().emplace<InputSystem>();
    // mesh初始化网格
    auto& meshManager = reg.ctx().emplace<MeshManager>();
    meshManager.GetQuadMesh();
    // shader管理器
    auto& shaderManager = reg.ctx().emplace<ShaderManager>();
    auto& shader = shaderManager.loadShader("default", "default.vs", "default.fs");
    spdlog::info("加载着色器{}", shader.getID());
    // 初始化渲染系统 （必须放在此处，依赖shader管理器和mesh管理器）
    reg.ctx().emplace<RenderSystem>(reg);
    // 纹理系统
    reg.ctx().emplace<TextureManager>();
    // 音频系统
    reg.ctx().emplace<AudioManager>();

    // 游戏播放器
    reg.ctx().emplace<cmd::CmdPlayer>();

    reg.ctx().emplace<ConfigManager>();

}

void gameStatusSet(entt::registry& reg)
{
    auto& audio = reg.ctx().get<AudioManager>();
    PlayerSys::createPlayer(reg);
    audio.playMusic("satori");
}
}