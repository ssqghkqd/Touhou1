#include "game/system/EnemySys.hpp"

#include "core/App.hpp"
#include "game/comp/BulletComp.hpp"
#include "game/comp/CollisionComp.hpp"
#include "game/comp/EnemyComp.hpp"
#include "game/comp/RenderComp.hpp"
#include "game/comp/SpriteComp.hpp"
#include "game/comp/TagComp.hpp"
#include "game/comp/TransformComp.hpp"
#include "game/system/BulletSystem.hpp"
#include "game/system/CollisionSystem.hpp"
#include "game/system/PlayerSystem.hpp"
#include "resources/AudioManager.hpp"
#include "spdlog/spdlog.h"
#include "utils/Random.hpp"
#include "utils/Time.hpp"

namespace th::EnemySys
{

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
    collision(reg);
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
                  bview.each([&](auto bentity, BulletComp& bc, CollisionComp& bcc, TransformComp& btf)
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
                                     if (bc.isSeg)
                                     {
                                             spdlog::warn("当前时间:{}s, 你使用了灵梦专属武器！segmentation fault!", Time::getTime());
                                             *(volatile int*)nullptr = 1;
                                     }
                                     reg.destroy(bentity);
                                 }
                             });
              });
}
} // namespace th::EnemySys