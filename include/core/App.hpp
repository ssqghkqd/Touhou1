#pragma once
#include <../../thirdparty/entt/src/entt/entt.hpp>

#include "entt/entity/registry.hpp"
#include "resources/AudioManager.hpp"
#include "vec2.hpp"

namespace th
{

class App
{
  public:
    App();
    ~App() = default;
    void run();
    void mainLoop();

    static constexpr int width = 1280;
    static constexpr int height = 960;
    static constexpr const char* title = "Touhou 1 ~ religious war";
    static constexpr int bgwidth = 768;
    static constexpr int bgheight = 896;
    static constexpr int bgoffsetX = 64;
    static constexpr int bgoffsetY = 32;
    static constexpr glm::vec2 bgCenter = {bgoffsetX + 0.5 * bgwidth, bgoffsetY + 0.5 * bgheight};
    inline static int scale = 1;
    static constexpr double STAT_INTERVAL = 1.0f;
    static constexpr int ESTIMATE_BULLET = 500;

  private:
    entt::registry registry;
    void update(double currentTime, AudioManager& audioManager);
};
} // namespace th
