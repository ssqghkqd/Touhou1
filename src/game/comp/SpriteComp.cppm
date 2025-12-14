// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
//
module;

export module game.comp:SpriteComp;
import glm;

namespace th
{
export struct SpriteComp
{
    glm::vec2 pos{0.0f, 0.0f};
    glm::vec2 vel{0.0f, 0.0f};
    glm::vec2 size{0.0f, 0.0f};
};
}
