#include "graphics/RenderSystem.hpp"

#include <format>

#include "core/App.hpp"
#include "graphics/MeshManager.hpp"
#include "graphics/ShaderManager.hpp"
#include "graphics/TextureManager.hpp"

namespace th
{

RenderSystem& RenderSystem::getInstance()
{
    static RenderSystem instance;
    if (!instance.inited)
        instance.init(App::width, App::height);
    return instance;
}

void RenderSystem::init(int screenWidth, int screenHeight)
{
    if (inited)
    {
        thLogger::warning("rendersystem已初始化");
        return;
    }

    thLogger::info("初始化渲染系统...");
    // 初始化着色器管理器
    auto& shaderManager = ShaderManager::getInstance();
    m_shader = &shaderManager.loadShader("default", "default.vs", "default.fs");
    thLogger::debug("着色器加载完成: " + std::to_string(m_shader->getID()));

    // 初始化纹理管理器
    auto& textureManager = TextureManager::getInstance();
    textureManager.loadTexture("player");
    textureManager.loadTexture("hitbox");
    textureManager.loadTexture("xiaoyu");
    textureManager.loadTexture("bg1");

    // 初始化网格管理器
    auto& meshManager = MeshManager::getInstance();
    m_quadMesh = &meshManager.GetQuadMesh();
    m_circleMesh = &meshManager.GetCircleMesh(16); // 16分段圆形

    // 设置OpenGL状态
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST); // 2D不需要深度测试
    thLogger::debug("OpenGL状态设置完成");

    // 创建投影矩阵
    setProjection(screenWidth, screenHeight);

    // 设置初始清屏颜色
    glClearColor(0.53f, 0.81f, 0.98f, 1.0f);
    thLogger::info("渲染系统初始化完成");
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

void RenderSystem::renderEntity(const SpriteComponent& tf, const RenderComponent& rc)
{
    // 不可见的直接返回
    if (!rc.isVisible)
        return;
    // 计算模型矩阵
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(tf.position, 0.0f));
    model = glm::rotate(model, tf.rotation, glm::vec3(0, 0, 1));    // 新增旋转
    model = glm::scale(model, glm::vec3(rc.size * tf.scale, 1.0f)); // 新增缩放

    // 设置着色器参数
    m_shader->use();
    m_shader->set("model", model);

    // 选择网格
    const MeshManager::Mesh* mesh = nullptr;
    switch (rc.meshType)
    {
        case MeshType::Circle:
            mesh = m_circleMesh;
            break;
        case MeshType::Rect:
        default:
            mesh = m_quadMesh;
    }

        // 绑定纹理
    if (!rc.textureName.empty())
    {
        m_shader->set("thTexture", 0);
        glActiveTexture(GL_TEXTURE0);
        TextureManager::getInstance().bind(rc.textureName);
    }

    // 渲染
    glBindVertexArray(mesh->vao);
    glDrawElements(GL_TRIANGLES, mesh->indexCount, GL_UNSIGNED_INT, 0);
}

/* void RenderSystem::renderPlayer(const TransformComponent &sprite)
{
    // 使用精灵着色器
    m_shader->use();

    // 计算模型矩阵
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(sprite.position, 0.0f));
    model = glm::scale(model, glm::vec3(sprite.size, 1.0f));

    // 设置着色器参数
    m_shader->set("model", model);
    m_shader->set("thTexture", 0); // 设置纹理单元

    // 绑定纹理
    glActiveTexture(GL_TEXTURE0);
    auto &textureManager = TextureManager::getInstance();
    textureManager.bind("player");

    // 渲染四边形
    glBindVertexArray(m_quadMesh->vao);
    glDrawElements(GL_TRIANGLES, m_quadMesh->indexCount, GL_UNSIGNED_INT, 0);
} */

void RenderSystem::renderHitbox(const SpriteComponent& playerTF, const HitboxComponent& hitbox, bool isSlowMode)
{
    if (!isSlowMode)
        return; // 只在低速模式显示
    // 计算判定点位置
    glm::vec2 hitboxPos = playerTF.position + hitbox.offset;

    // 创建临时渲染组件
    RenderComponent rc;
    rc.meshType = MeshType::Rect;
    rc.textureName = hitbox.textureName;
    rc.size = glm::vec2(hitbox.renderRadius * 2);

    // 创建临时变换组件
    SpriteComponent tf;
    tf.position = hitboxPos;

    // std::cout << std::format("纹理:{}, 位置:{} {}\n", rc.textureName,
    // sc.position.x, sc.position.y);
    //  使用通用渲染函数
    renderEntity(tf, rc);
}

/*
void RenderSystem::renderBullet(const TransformComponent &sc,
                                const BulletRenderComponent &render)
{
    // 使用简单着色器
    m_shader->use();

    // 计算模型矩阵
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(sc.position, 0.0f));
    model = glm::scale(model, glm::vec3(render.size, render.size, 1.0f));

    // 设置着色器参数
    m_shader->set("model", model);

    // 绑定纹理
    glActiveTexture(GL_TEXTURE0);
    static auto &textureManager = TextureManager::getInstance();
    textureManager.bind("xiaoyu");

    // 渲染圆形弹幕
    glBindVertexArray(m_circleMesh->vao);
    glDrawElements(GL_TRIANGLES, m_circleMesh->indexCount, GL_UNSIGNED_INT, 0);
} */

void RenderSystem::renderBackground()
{
    // 创建背景实体（不注册到ECS）
    static SpriteComponent bgtf;
    static RenderComponent bgRender;

    bgtf.position = glm::vec2(App::width / 2, App::height / 2);
    bgRender.textureName = "bg1";
    bgRender.size = glm::vec2(App::width, App::height);

    renderEntity(bgtf, bgRender);
}

void RenderSystem::update(entt::registry& registry)
{
    // 清除屏幕
    glClear(GL_COLOR_BUFFER_BIT);
    // thLogger::debug("rendersystem: 清屏");

    // 注意先渲染背景

    renderBackground();

    /* // 渲染所有玩家 entt::entity entity, PlayerControl &control,
    TransformComponent &sc auto playerView = registry.view<TransformComponent,
    TransformComponent, PlayerTag>(); playerView.each([&](entt::entity entity,
    TransformComponent &sc) { renderPlayer(sc); });

    // 渲染所有判定点
    auto hitboxView = registry.view<TransformComponent, HitboxComponent,
    PlayerControl>(); hitboxView.each([&](entt::entity entity, TransformComponent
    &sc, HitboxComponent &hitbox, PlayerControl &control)
                    {
    //thLogger::debug("渲染判定点实体: " +
    std::to_string(entt::to_integral(entity))); renderHitbox(sc, hitbox,
    control.slowMode); });

    // 渲染弹幕
    auto bulletView = registry.view<TransformComponent, BulletRenderComponent>();
    bulletView.each([&](entt::entity entity, TransformComponent &sc,
    BulletRenderComponent &render) { renderBullet(sc, render); }); */

    // 2. 弹幕（应该在玩家前面）
    auto bulletView = registry.view<SpriteComponent, RenderComponent, BulletTag>();
    bulletView.each([&](auto entity, SpriteComponent& tf, RenderComponent& rc)
                    {
                        renderEntity(tf, rc);
                    });

    // 3. 玩家
    auto playerView = registry.view<SpriteComponent, RenderComponent, PlayerTag>();
    playerView.each([&](auto entity, SpriteComponent& tf, RenderComponent& rc)
                    {
                        renderEntity(tf, rc);
                    });

    // 4. 判定点（最上层）
    auto hitboxView = registry.view<SpriteComponent, HitboxComponent, PlayerControl>();
    hitboxView.each([&](auto entity, SpriteComponent& tf, HitboxComponent& hitbox, PlayerControl& pc)
                    {
                        renderHitbox(tf, hitbox, pc.slowMode);
                    });
}
} // namespace th