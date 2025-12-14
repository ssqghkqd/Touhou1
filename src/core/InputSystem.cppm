// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
// 输入系统
module;
export module core:InputSystem;
import glm;
import entt;
import :Window;
import defs;

namespace th::InputSystem
{

export void update(entt::registry& reg)
{
    const auto& window = reg.ctx().get<Window>();
    defs::InputState currentState{};

    currentState.keyStates.w = window.isKeyPressed(defs::Key::w);
    currentState.keyStates.s = window.isKeyPressed(defs::Key::s);
    currentState.keyStates.d = window.isKeyPressed(defs::Key::d);
    currentState.keyStates.a = window.isKeyPressed(defs::Key::a);
    currentState.keyStates.left_alt = window.isKeyPressed(defs::Key::left_alt);
    currentState.keyStates.left_control = window.isKeyPressed(defs::Key::left_control);
    currentState.keyStates.left_shift = window.isKeyPressed(defs::Key::left_shift);
    currentState.keyStates.space = window.isKeyPressed(defs::Key::space);
    currentState.keyStates.esc = window.isKeyPressed(defs::Key::ecs);

    currentState.mouseStates.x = window.getMousePos().x;
    currentState.mouseStates.y = window.getMousePos().y;
    currentState.mouseStates.left = window.isButtonPressed(defs::Button::left);
    currentState.mouseStates.right = window.isButtonPressed(defs::Button::right);
    reg.ctx().get<entt::dispatcher>().trigger(currentState);
}
} // namespace th::InputSystem
