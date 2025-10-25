#pragma once

#include <entt/entt.hpp>

#include "glm.hpp"
#include "graphics/MeshManager.hpp"
#include "graphics/Shader.hpp"

namespace th
{
struct PlayerComp;
struct RenderComp;
struct TransformComp;

class RenderSystem
{
  public:
    ~RenderSystem() = default;
    static RenderSystem& getInstance();

    void init(int screenWidth, int screenHeight);
    void update(entt::registry& registry);
    void setProjection(int width, int height);

    /* void renderPlayer(const TransformComponent &sprite); */
    void renderHitbox(const TransformComp& playerTF, bool isSlowMode, const PlayerComp& pc) const;
    /*void renderBullet(const TransformComponent &sc,
                      const BulletRenderComponent &render); */
    void renderBackground() const;

    void renderEntity(TransformComp& tf, RenderComp& rc) const;

    // 删除拷贝构造和赋值操作符
    RenderSystem(const RenderSystem&) = delete;
    RenderSystem& operator=(const RenderSystem&) = delete;

    // 移动语义也删除（单例不需要）
    RenderSystem(RenderSystem&&) = delete;
    RenderSystem& operator=(RenderSystem&&) = delete;

  private:
    RenderSystem() = default;
    glm::mat4 m_projection{};
    bool inited = false;

    const Shader* m_shader = nullptr;
    const MeshManager::Mesh* m_quadMesh = nullptr;
    const MeshManager::Mesh* m_circleMesh = nullptr;
};
} // namespace th
