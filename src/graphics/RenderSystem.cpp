module;
#include <entt/entt.hpp>
#include <glm/ext.hpp>
module graphics.RenderSystem;
import Config;
import spdlog;
import opengl;
import game.system.PlayerSys;
import game.comp;
import graphics.ShaderManager;
import graphics.TextureManager;

namespace th
{

RenderSystem::RenderSystem(entt::registry& registry)
{
    init(registry, window_width, window_height);
}


void RenderSystem::init(entt::registry& registry, const int screenWidth, const int screenHeight)
{
    if (inited)
    {
        spdlog::warn("RenderSystem already initialized");
        return;
    }

    auto& shaderManager = registry.ctx().get<ShaderManager>();
    m_shader = &shaderManager.getShader("default");

    auto& meshManager = registry.ctx().get<MeshManager>();
    m_quadMesh = &meshManager.GetQuadMesh();

    // 设置OpenGL状态
    gl::enable(gl::blend);
    gl::blendFunc(gl::src_alpha, gl::one_minus_src_alpha);
    gl::disable(gl::depth_test); // 2D不需要深度测试
    spdlog::debug("OPENGL 状态设置完成");

    setProjection(screenWidth, screenHeight);

    gl::clearColor(0.53f, 0.81f, 0.98f, 1.0f);
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
    if (!rc.isVisible)
        return;
    auto model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(tf.position, 0.0f));
    model = glm::rotate(model, tf.rotation, glm::vec3(0, 0, 1));
    model = glm::scale(model, glm::vec3(rc.size * tf.scale, 1.0f));

    m_shader->use();
    m_shader->set("model", model);
    m_shader->set("entityColor", rc.color);

    const MeshManager::Mesh* mesh = m_quadMesh;

    // TODO 优化批处理
    /*
     * 这里目前暂时无需优化 但是一个性能热点 目前帧率2000的情况下不管
     */
    if (!rc.textureName.empty())
    {
        m_shader->set("thTexture", 0);
        gl::activeTexture(gl::texture0);
        registry.ctx().get<TextureManager>().bind(rc.textureName);
    }

    // TODO 这里可以用实例化渲染 但目前还是不用并且我不知道实例化渲染需要准备什么（
    gl::bindVertexArray(mesh->vao);
    gf::drawElements(gl::triangles, mesh->indexCount, gl::unsigned_int, nullptr);
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

    bgtf.position = glm::vec2(window_width / 2, window_height / 2);
    bgRender.textureName = "bg1";
    bgRender.size = glm::vec2(window_width, window_height);

    renderEntity(registry, bgtf, bgRender);
}
void RenderSystem::update(entt::registry& registry) const
{
    // 清除屏幕
    gl::clear(gl::color_buffer_bit);

    // 注意先渲染背景

    renderBackground(registry);

    // 2. 弹幕 敌人（应该在玩家前面）
    auto view = registry.view<TransformComp, RenderComp>(entt::exclude<PlayerTag>);
    view.each([&registry, this](auto& tf, auto& rc)
                    {
                        renderEntity(registry, tf, rc);
                    });

    // 3. 玩家
    static auto& m_player = PlayerSys::getPlayer();
    auto& tf = registry.get<TransformComp>(m_player);
    auto& rc = registry.get<RenderComp>(m_player);
    const auto& pc = registry.get<PlayerComp>(m_player);
    renderEntity(registry, tf, rc);
    renderHitbox(registry, tf, pc.isSlow, pc);
}
} // namespace th