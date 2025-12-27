// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
// 一个专门处理全局性、非游戏性输入逻辑的系统
module;

export module core:AppLogicSys;
import entt;
import defs;
import spdlog;
import :Window;

namespace th::AppLogic
{
struct AppHandler
{
    Window& window;
    /*entt要求有这个参数 所以加上maybeunused*/
    void handleAppShutDownRequestEvent([[maybe_unused]] const defs::AppShutDownRequestEvent& event) const
    {
        window.closeWindow();
    }
    void handleWindowToggleCursorEvent([[maybe_unused]] const defs::WindowToggleCursorEvent& event) const
    {
        window.toggleCursor();
    };
};

struct Handler
{
    entt::dispatcher& dp;
    defs::InputState m_prevState{};
    void handleInput(const defs::InputState& event)
    {
        if (event.keyStates.esc)
        {
            dp.trigger(defs::AppShutDownRequestEvent{});
        }
        // 按键防抖
        if (event.keyStates.left_alt && !m_prevState.keyStates.left_alt)
        {
            dp.trigger(defs::WindowToggleCursorEvent{});
        }
        m_prevState = event;
    }
};

export void registerHandle(entt::registry& reg)
{
    auto& dp = reg.ctx().get<entt::dispatcher>();
    static Handler handler{dp};
    dp.sink<defs::InputState>().connect<&Handler::handleInput>(handler);
    auto& window = reg.ctx().get<Window>();
    static AppHandler appHandler{window};
    dp.sink<defs::AppShutDownRequestEvent>()
        .connect<&AppHandler::handleAppShutDownRequestEvent>(appHandler);
    dp.sink<defs::WindowToggleCursorEvent>()
        .connect<&AppHandler::handleWindowToggleCursorEvent>(appHandler);
}
} // namespace th::AppLogic
