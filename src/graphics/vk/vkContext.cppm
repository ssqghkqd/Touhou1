// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
//
module;

export module vk:vkContext;
import :vkcore;

namespace th
{
export class vkContext
{
  private:
    vkcore core_{};

  public:
    vkContext() = default;
    ~vkContext() = default;
};
} // namespace th
