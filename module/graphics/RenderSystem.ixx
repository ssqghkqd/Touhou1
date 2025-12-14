//
// Created by ss on 2025/11/22.
//
module;
#include <entt/entt.hpp>
#include <glm/glm.hpp>
export module graphics.RenderSystem;
import graphics.MeshManager;
import graphics.Shader;
import game.comp;

export namespace th
{
class RenderSystem
{
public:
    ~RenderSystem() = default;

    void init(entt::registry& registry, int screenWidth, int screenHeight);
    void update(entt::registry& registry) const;
    void setProjection(int width, int height);

    /* void renderPlayer(const TransformComponent &sprite); */
    void renderHitbox(entt::registry& registry, const TransformComp& playerTF, bool isSlowMode, const PlayerComp& pc) const;
    /*void renderBullet(const TransformComponent &sc,
                      const BulletRenderComponent &render); */
    void renderBackground(entt::registry& registry) const;

    void renderEntity(entt::registry& registry, TransformComp& tf, RenderComp& rc) const;

    // 删除拷贝构造和赋值操作符
    RenderSystem(const RenderSystem&) = delete;
    RenderSystem& operator=(const RenderSystem&) = delete;

    // 移动语义也删除（单例不需要）
    RenderSystem(RenderSystem&&) = delete;
    RenderSystem& operator=(RenderSystem&&) = delete;

    explicit RenderSystem(entt::registry& registry);

private:
    glm::mat4 m_projection{};
    bool inited = false;

    const Shader* m_shader = nullptr;
    const MeshManager::Mesh* m_quadMesh = nullptr;
};
} // namespace th