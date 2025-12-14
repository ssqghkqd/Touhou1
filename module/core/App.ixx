//
// Created by ss on 2025/11/22.
//
module;
#include <entt/entt.hpp>
export module core.App;
import resources.AudioManager;

export namespace th
{

class App
{
  public:
    App();
    ~App() = default;
    void run();
    void mainLoop();

  private:
    entt::registry m_registry;
    static void update(AudioManager& audioManager);
};
} // namespace th
