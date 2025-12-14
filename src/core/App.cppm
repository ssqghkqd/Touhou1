// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
// 主类 应用程序的生命周期管理a
module;
#include <cstdint>
#include <string>
export module core:App;
import entt;
import spdlog;

import :Init;
import :InputSystem;

import defs;

namespace th
{
struct AppHandler
{
    Window& window;
    void handleAppShutDownRequestEvent([[maybe_unused]] const defs::AppShutDownRequestEvent& event) const
    {
        window.closeWindow();
    }
    void handleWindowToggleCursorEvent([[maybe_unused]] const defs::WindowToggleCursorEvent& event) const
    {
        window.toggleCursor();
    };
};

export class App
{
  private:
    entt::registry registry_{};
    std::string errorInfor_{};
    bool initFailed_{false};

  public:
    App()
    {
        const auto initPoss = Init::init(registry_);
        if (initPoss.has_value())
        {
            errorInfor_ = defs::errorStr[(uint32_t)initPoss.value()];
            initFailed_ = true;
            return;
        }
        registerEvent();
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

        while (!window.shouldClose())
        {
            Time::update();
            window.pollEvents();
            window.updateFPS(Time::getTime());

            InputSystem::update(registry_);
        }
    }

    void registerEvent()
    {
        auto& dp = registry_.ctx().get<entt::dispatcher>();
        auto& window = registry_.ctx().get<Window>();
        static AppHandler appHandler{window};
        dp.sink<defs::AppShutDownRequestEvent>()
            .connect<&AppHandler::handleAppShutDownRequestEvent>(appHandler);
        dp.sink<defs::WindowToggleCursorEvent>()
            .connect<&AppHandler::handleWindowToggleCursorEvent>(appHandler);
    }
};
} // namespace mc
