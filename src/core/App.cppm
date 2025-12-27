// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
// 主类 应用程序的生命周期管理a
module;
#include <cstdint>
#include <thread>
export module core:App;
import entt;
import spdlog;

import :Init;
import :InputSystem;

import defs;

namespace th
{
export class App
{
  private:
    entt::registry registry_{};
    const char* errorInfor_{};
    bool initFailed_{false};

  public:
    App() = default;
    void init()
    {
        const auto e = Init::init(registry_);
        if (e.has_value())
        {
            errorInfor_ = defs::errorStr[(uint32_t)e.value()];
            initFailed_ = true;
            return;
        }
    }
    ~App() = default; // 自动清理资源

    int run()
    {
        if (initFailed_)
        {
            spdlog::critical("致命错误 退出程序:{}", errorInfor_);
            return 1;
        }
        Time::gameStart();
        spdlog::debug("游戏开始");
        mainloop();
        return 0;
    }

    void mainloop()
    {
        auto& window = registry_.ctx().get<Window>();
        auto& render = registry_.ctx().get<vk::Renderer>();

        while (!window.shouldClose())
        {
            Time::update();
            window.pollEvents();
            window.updateFPS(Time::getTime());

            render.render();

            InputSystem::update(registry_);
        }
    }
};
} // namespace th
