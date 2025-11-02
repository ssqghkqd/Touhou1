#include "ecs/system/EnemySys.hpp"

#include "core/App.hpp"
#include "ecs/comp/BulletComp.hpp"
#include "ecs/comp/CollisionComp.hpp"
#include "ecs/comp/EnemyComp.hpp"
#include "ecs/comp/RenderComp.hpp"
#include "ecs/comp/SpriteComp.hpp"
#include "ecs/comp/TagComp.hpp"
#include "ecs/comp/TransformComp.hpp"
#include "ecs/system/BulletSystem.hpp"
#include "ecs/system/CollisionSystem.hpp"
#include "ecs/system/PlayerSystem.hpp"
#include "resources/AudioManager.hpp"
#include "spdlog/spdlog.h"
#include "utils/Random.hpp"

namespace th::EnemySys
{

static float timer = 0.0f;
entt::entity spawnEnemy(entt::registry& reg, glm::vec2 pos)
{
    spdlog::info("创建敌人在({},{})", pos.x, pos.y);
    auto enemy = reg.create();
    auto& tf = reg.emplace<TransformComp>(enemy);
    auto& sc = reg.emplace<SpriteComp>(enemy);
    auto& rc = reg.emplace<RenderComp>(enemy);
    auto& cc = reg.emplace<CollisionComp>(enemy);
    auto& ec = reg.emplace<EnemyComp>(enemy);
    reg.emplace<EnemyTag>(enemy);

    tf.position = pos;
    rc.textureName = "enemy";
    rc.size = glm::vec2(64.0f);
    cc.radius = 10.0f;

    return enemy;
}

void update(entt::registry& reg, float dt)
{
    // 随机测试功能

    collision(reg);
    timer += dt;
    const auto view = reg.view<EnemyComp, SpriteComp, TransformComp, EnemyTag>();
    view.each([&](auto entity, EnemyComp& ec, SpriteComp& sc, TransformComp& tf)
              {
                  if (glm::length(ec.target_pos - tf.position) < 40.0f)
                  {
                      sc.velocity = glm::vec2(0.0f);
                  }
                  if (timer < ec.move_cd)
                  {
                      if (timer < 1.0f)
                      {
                          return;
                      }
                      homing(reg);
                  }
                  timer = 0.0f;
                  const float x = Random::range(App::bgoffsetX + 10.0f,
                                                App::bgoffsetX + App::bgwidth - 10.0f);
                  const float y = Random::range(App::bgoffsetY + 50.0f,
                                                App::bgoffsetY + 100.0f);
                  ec.target_pos = {x, y};
                  glm::vec dir = glm::normalize(glm::vec2(x, y) - tf.position);
                  sc.velocity = dir * 50.0f;
              });

    // 随机测试功能
}

void homing(entt::registry& reg)
{
    static const auto m_player = PlayerSystem::getPlayer();
    const auto view = reg.view<TransformComp, EnemyTag>();
    view.each([&](auto entity, auto& tf)
              {
                  auto& tfp = reg.get<TransformComp>(m_player);
                  glm::vec dir = glm::normalize(tfp.position - tf.position);
                  BulletSystem::createBullet(reg, tf.position, dir * 200.0f);
                  // spdlog::info("敌人发射弹幕 位置({},{})速度({},{})", tf.position.x, tf.position.y, dir.x * 200.0f, dir.y * 200.0f);
              });
}
void collision(entt::registry& reg)
{
    auto& audio = reg.ctx().get<AudioManager>();
    const auto view = reg.view<EnemyComp, CollisionComp, TransformComp, EnemyTag>();
    view.each([&](auto entity, auto& ec, auto& cc, auto& tf)
              {
                  if (ec.hp <= 0.0f)
                  {
                      reg.destroy(entity);
                      audio.playSound("enemy_death");
                      return;
                  }
                  const auto bview = reg.view<BulletComp, CollisionComp, TransformComp, BulletTag>();
                  bview.each([&](auto bentity, auto& bc, auto& bcc, auto& btf)
                             {
                                 if (!bc.isPlayer)
                                 {
                                     return;
                                 }
                                 if (CollisionSystem::checkCollision(tf.position, cc.radius, btf.position, bcc.radius))
                                 {
                                     ec.hp -= 1.0f;
                                     audio.playSound("enemy_shot");
                                     spdlog::info("击中敌人！当前血量{}", ec.hp);
                                     reg.destroy(bentity);
                                 }
                             });
              });
}
} // namespace th::EnemySys