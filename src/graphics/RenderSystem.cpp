#include "graphics/RenderSystem.hpp"

#define gfDrawElements glDrawElements
#include "core/App.hpp"
#include "ext.hpp"
#include "game/comp/PlayerComp.hpp"
#include "game/comp/RenderComp.hpp"
#include "game/comp/TransformComp.hpp"
#include "game/system/PlayerSystem.hpp"
#include "glad.h"
#include "graphics/MeshManager.hpp"
#include "graphics/ShaderManager.hpp"
#include "graphics/TextureManager.hpp"
#include "spdlog/spdlog.h"

namespace th
{

RenderSystem::RenderSystem(entt::registry& registry)
{
    init(registry, App::width, App::height);
}


void RenderSystem::init(entt::registry& registry, const int screenWidth, const int screenHeight)
{
    if (inited)
    {
        spdlog::warn("RenderSystem already initialized");
        return;
    }

    // 获取纹理管理器
    auto& shaderManager = registry.ctx().get<ShaderManager>();
    m_shader = &shaderManager.getShader("default");


    // 初始化网格管理器
    auto& meshManager = registry.ctx().get<MeshManager>();
    m_quadMesh = &meshManager.GetQuadMesh();

    // 设置OpenGL状态
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST); // 2D不需要深度测试
    spdlog::debug("OPENGL 状态设置完成");

    // 创建投影矩阵
    setProjection(screenWidth, screenHeight);

    // 设置初始清屏颜色
    glClearColor(0.53f, 0.81f, 0.98f, 1.0f);
    spdlog::info("渲染系统初始化完成");
    inited = true;
}

void RenderSystem::setProjection(int width, int height)
{
    // 正交投影矩阵 (Y轴向下，适合2D)
    m_projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f, -1.0f, 1.0f);

    // 更新着色器
    m_shader->use();
    m_shader->set("projection", m_projection);
}

void RenderSystem::renderEntity(entt::registry& registry, TransformComp& tf, RenderComp& rc) const
{
    // 不可见的直接返回
    if (!rc.isVisible)
        return;
    // 计算模型矩阵
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(tf.position, 0.0f));
    model = glm::rotate(model, tf.rotation, glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(rc.size * tf.scale, 1.0f));

    // 设置着色器参数
    m_shader->use();
    m_shader->set("model", model);
    m_shader->set("entityColor", rc.color);

    // 选择网格（现在不选了
    const MeshManager::Mesh* mesh = m_quadMesh;

    // TODO 优化批处理
    /*
     * 这里目前暂时无需优化 但是一个性能热点 目前帧率2000的情况下不管
     */
    if (!rc.textureName.empty())
    {
        m_shader->set("thTexture", 0);
        glActiveTexture(GL_TEXTURE0);
        registry.ctx().get<TextureManager>().bind(rc.textureName);
    }

    // 渲染
    glBindVertexArray(mesh->vao);
    gfDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, nullptr);
}

void RenderSystem::renderHitbox(entt::registry& registry, const TransformComp& playerTF, bool isSlowMode, const PlayerComp& pc) const
{
    if (!isSlowMode)
        return; // 只在低速模式显示
    // 计算判定点位置
    const glm::vec2 hitboxPos = playerTF.position + pc.hitboxOffset;

    // 创建临时渲染组件
    RenderComp rc;
    rc.textureName = "hitbox";
    rc.size = pc.hitboxSize;

    // 创建临时变换组件
    TransformComp tf;
    tf.position = hitboxPos;

    //  使用通用渲染函数
    renderEntity(registry, tf, rc);
}

void RenderSystem::renderBackground(entt::registry& registry) const
{
    // 创建背景实体（不注册到ECS）
    static TransformComp bgtf;
    static RenderComp bgRender;

    bgtf.position = glm::vec2(App::width / 2, App::height / 2);
    bgRender.textureName = "bg1";
    bgRender.size = glm::vec2(App::width, App::height);

    renderEntity(registry, bgtf, bgRender);
}

#undef gfDrawElements
void RenderSystem::update(entt::registry& registry) const
{
    // 清除屏幕
    glClear(GL_COLOR_BUFFER_BIT);
    // thLogger::debug("rendersystem: 清屏");

    // 注意先渲染背景

    renderBackground(registry);

    // 2. 弹幕 敌人（应该在玩家前面）
    auto view = registry.view<TransformComp, RenderComp>();
    view.each([&](auto entity, auto& tf, auto& rc)
                    {
                        renderEntity(registry, tf, rc);
                    });

    // 3. 玩家
    static auto& m_player = PlayerSystem::getPlayer();
    auto& tf = registry.get<TransformComp>(m_player);
    auto& rc = registry.get<RenderComp>(m_player);
    const auto& pc = registry.get<PlayerComp>(m_player);
    renderEntity(registry, tf, rc);
    renderHitbox(registry, tf, pc.isSlow, pc);
}
} // namespace th