// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
// 事件s
module;

export module defs:Events;

namespace th::defs
{
export struct InputState
{
    // 当前帧所有按键的状态 作为一个事件
    struct
    {
        bool w{false};
        bool a{false};
        bool s{false};
        bool d{false};
        bool space{false};
        bool left_shift{false};
        bool left_alt{false};
        bool left_control{false};
        bool esc{false};

    } keyStates;

    struct
    {
        bool left{false};
        bool right{false};
        float x{0.0f};
        float y{0.0f};
    } mouseStates;

};

export struct AppShutDownRequestEvent
{
};
export struct WindowToggleCursorEvent
{
};

} // namespace mc::impl::events
