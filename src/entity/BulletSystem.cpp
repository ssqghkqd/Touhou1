#include "entity/BulletSystem.hpp"

#include "core/App.hpp"
#include "entity/bullet/HomingBulletSystem.hpp"
#include "entity/component/BulletComponent.hpp"
#include "entity/component/RenderComponent.hpp"
#include "entity/component/SpriteComponent.hpp"
#include "utils/Logger.hpp"
#include "utils/Random.hpp"

namespace th
{

BulletSystem& BulletSystem::getInstance()
{
    static BulletSystem instance;
    return instance;
}

void BulletSystem::init(entt::registry& registry)
{
    if (inited)
    {
        thLogger::warning("bulletsystem已初始化");
        return;
    }

    // 这里添加初始化过程
    HomingBulletSystem::getInstance().init(registry);

    thLogger::info("bulletsystem初始化");
    inited = true;
}

void BulletSystem::update(entt::registry& registry, float deltaTime)
{
    // 这里是测试的随机弹幕
    static auto& hmbs = HomingBulletSystem::getInstance();
    // 更新计时器
    spawnTimer += deltaTime;

    // 检查是否应该生成新弹幕
    if (spawnTimer >= spawnInterval)
    {
        spawnTimer = 0.0f;
        hmbs.createHoming(registry, 10, 120.0f);
        for (int i = 0; i < 5; i++)
        {
            spawnRandomBullet(registry);
        }
    }

    // 这里是测试的随机弹幕

    // 获取所有弹幕实体
    static auto view = registry.view<BulletComponent, TransformComponent>();

    view.each([&](entt::entity entity, BulletComponent& bullet, TransformComponent& tf)
              {
                  // 应用运动
                  tf.position += bullet.velocity * deltaTime;

                  // 边界检查
                  if (tf.position.x > App::bgoffsetX + App::bgwidth || tf.position.x < App::bgoffsetX ||
                      tf.position.y > App::bgoffsetY + App::bgheight || tf.position.y < App::bgoffsetY)
                  {
                      registry.destroy(entity);
                      thLogger::debug("超出屏幕的弹幕已被删除 在(" + std::to_string(tf.position.x) + ", " +
                                      std::to_string(tf.position.y) + ')');
                      return;
                  }

                  if (!bullet.isExistForever)
                  {
                      // 更新生命周期
                      bullet.lifetime -= deltaTime;
                      // 移除过期弹幕
                      if (bullet.lifetime <= 0.0f)
                      {
                          registry.destroy(entity);
                          thLogger::info("删除弹幕");
                      }
                  }
              });
}

// 测试函数
void BulletSystem::spawnRandomBullet(entt::registry& registry)
{
    // 随机生成位置（屏幕上方）
    float x = Random::range(App::bgoffsetX, App::bgoffsetX + App::bgwidth);
    float y = App::bgoffsetY + 10.0f;

    // 固定速度向下
    glm::vec2 velocity(Random::range(-50.0f, 50.0f), Random::range(250.0f, 450.0f));

    createBullet(registry, {x, y}, velocity);
}

entt::entity BulletSystem::createBullet(entt::registry& registry, const glm::vec2& position, const glm::vec2& velocity, bool isPlayerBullet, bool isExistForever)
{
    auto bullet = registry.create();

    // 变换组件
    auto& tf = registry.emplace<TransformComponent>(bullet);
    tf.position = position;

    // 弹幕组件
    auto& bulletComp = registry.emplace<BulletComponent>(bullet);
    bulletComp.velocity = velocity;
    bulletComp.speed = glm::length(velocity);
    bulletComp.isPlayerBullet = isPlayerBullet;
    bulletComp.isExistForever = isExistForever;

    // 渲染组件
    auto& render = registry.emplace<RenderComponent>(bullet);
    render.textureName = "xiaoyu";
    render.meshType = MeshType::Circle;
    render.size = glm::vec2(32.0f);

    // 碰撞组件（未来启用）
    registry.emplace<BulletCollider>(bullet);

    // 标签
    registry.emplace<BulletTag>(bullet);

    return bullet;
}

} // namespace th
