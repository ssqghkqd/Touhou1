// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
// stbi包装
module;
#include <stb/stb_image.h>
export module stb_image;

export namespace stbi
{
auto& load = stbi_load;
auto& failureReason = stbi_failure_reason;
auto& imageFree = stbi_image_free;
}