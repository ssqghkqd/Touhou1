// ecs/system/SpriteMovementSys.cpp
module;
#include <entt/entt.hpp>
module game.system.SpriteMovementSys;
import spdlog;
import Config;
import game.comp;


namespace th::SpriteMovementSys
{

void update(entt::registry& registry, float dt)
{
    static auto view = registry.view<TransformComp, SpriteComp, RenderComp>(entt::exclude<PlayerTag>);

    view.each([&registry, dt](auto entity, auto& tf, auto& sprite, auto& render)
              {
                  tf.position += sprite.velocity * dt;

                  const float halfWidth = render.size.x * 0.5f;
                  const float halfHeight = render.size.y * 0.5f;

                  // 超出边界的删除
                  if (tf.position.x + halfWidth > bg_offset_x + bg_width || tf.position.x - halfWidth < bg_offset_x ||
                      tf.position.y + halfHeight > bg_offset_y + bg_height || tf.position.y - halfHeight < bg_offset_y)
                  {
                      spdlog::debug("删除实体在({},{})", tf.position.x, tf.position.y);
                      registry.destroy(entity);
                  }
              });
}
} // namespace th::SpriteMovementSys