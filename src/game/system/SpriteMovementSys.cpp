// ecs/system/SpriteMovementSys.cpp

#include <entt/entt.hpp>

#include "core/App.hpp"
#include "game/comp/RenderComp.hpp"
#include "game/comp/SpriteComp.hpp"
#include "game/comp/TagComp.hpp"
#include "game/comp/TransformComp.hpp"
#include "spdlog/spdlog.h"

namespace th::SpriteMovementSys
{

void update(entt::registry& registry, float dt)
{
    static auto view = registry.view<TransformComp, SpriteComp, RenderComp>(entt::exclude<PlayerTag>);

    view.each([&](auto entity, auto& tf, auto& sprite, auto& render)
              {
                  tf.position += sprite.velocity * dt;

                  const float halfWidth = render.size.x * 0.5f;
                  const float halfHeight = render.size.y * 0.5f;

                  // 超出边界的删除
                  if (tf.position.x + halfWidth > App::bgoffsetX + App::bgwidth || tf.position.x - halfWidth < App::bgoffsetX ||
                      tf.position.y + halfHeight > App::bgoffsetY + App::bgheight || tf.position.y - halfHeight < App::bgoffsetY)
                  {
                      spdlog::debug("删除实体在({},{})", tf.position.x, tf.position.y);
                      registry.destroy(entity);
                  }
              });
}
} // namespace th::SpriteMovementSys