module;
#include <entt/entt.hpp>
#include <glm/ext.hpp>
module game.system.PlayerSys;
import nlohmann_json;
import utils.JsonManager;
import Config;
import game.system.BulletSys;
import utils.Time;
import core.Window;
import glfw;
import game.comp;

namespace th::PlayerSys
{

static entt::entity m_player;
static float shotInterval = 0.1f;
static float m_lastTime = 0.0f;
void update(entt::registry& registry, float dt)
{
    static nlohmann::json& playerJ = JsonManager::get("config.player");
    constexpr int width = bg_offset_x + bg_width;
    constexpr int height = bg_offset_y + bg_height;
    static float slowMoveSpeed = playerJ["slow_speed"];
    static float moveSpeed = playerJ["speed"];

    const auto& player = registry.get<PlayerComp>(m_player);
    auto& sprite = registry.get<SpriteComp>(m_player);
    auto& drag = registry.get<DragComp>(m_player);
    auto& render = registry.get<RenderComp>(m_player);
    auto& tf = registry.get<TransformComp>(m_player);

    // 1. 计算目标速度
    float targetSpeed = player.isSlow ? slowMoveSpeed : moveSpeed;
    glm::vec2 targetVelocity = player.targetDir * targetSpeed;

    // 使用线性插值平滑过渡速度
    sprite.velocity = glm::mix(sprite.velocity, targetVelocity, drag.smoothFactor);

    // 应用速度到位置
    tf.position += sprite.velocity * dt;

    // 边界检查（考虑精灵尺寸）

    const float halfWidth = render.size.x / 2.0f;
    const float halfHeight = render.size.y / 2.0f;

    tf.position.x = glm::clamp(tf.position.x, bg_offset_x + halfWidth, (float)width - halfWidth);
    tf.position.y = glm::clamp(tf.position.y, bg_offset_y + halfHeight, (float)height - halfHeight);
}

entt::entity createPlayer(entt::registry& registry)
{
    static nlohmann::json& playerJ = JsonManager::get("config.player");
    const auto player = registry.create();

    auto& tf = registry.emplace<TransformComp>(player);
    // 精灵组件
    [[maybe_unused]] auto& sprite = registry.emplace<SpriteComp>(player);

    // 玩家控制组件
    [[maybe_unused]] auto& playerComp = registry.emplace<PlayerComp>(player);

    auto& render = registry.emplace<RenderComp>(player);
    [[maybe_unused]] auto& drag = registry.emplace<DragComp>(player);
    auto& cs = registry.emplace<CollisionComp>(player);

    // 玩家标签
    registry.emplace<PlayerTag>(player);

    tf.position = {bg_width / 2.0f, bg_height / 2.0f}; // 屏幕中心
    float width = playerJ["width"];
    float height = playerJ["height"];
    auto textureName = playerJ["texture_name"];
    render.size = {width, height}; // 适当尺寸
    render.textureName = textureName;
    cs.radius = playerJ["collision_radius"];

    m_player = player;
    return player;
}

void shot(entt::registry& registry)
{
    const float currentTime = Time::getTime();

    if (currentTime - m_lastTime >= shotInterval)
    {
        m_lastTime = currentTime;
        auto& tf = registry.get<TransformComp>(m_player);
        BulletSys::createBullet(registry, tf.position, {0.0f, -1000.0f}, "bullet_default", true);
    }
}

entt::entity& getPlayer()
{
    return m_player;
}

void updatePlayerMovement(entt::registry& registry)
{
    auto& pc = registry.get<PlayerComp>(m_player);
    // 检测低速模式
    static const auto& window = registry.ctx().get<Window>();
    pc.isSlow = window.isKeyPressed(glfw::key_left_shift);

    pc.targetDir = glm::vec2(0.0f); // 重置为零向量

    // 移动方向
    if (window.isKeyPressed(glfw::key_up))
        pc.targetDir.y = -1.0f;
    if (window.isKeyPressed(glfw::key_down))
        pc.targetDir.y = 1.0f;
    if (window.isKeyPressed(glfw::key_left))
        pc.targetDir.x = -1.0f;
    if (window.isKeyPressed(glfw::key_right))
        pc.targetDir.x = 1.0f;

    // 标准化方向
    if (glm::length(pc.targetDir) > 0.0f)
    {
        pc.targetDir = glm::normalize(pc.targetDir);
    }
}

} // namespace th::PlayerSystem
