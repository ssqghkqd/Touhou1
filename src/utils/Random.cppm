// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
// 一些随机数生成器
module;
#include <random>
export module utils:Random;

namespace th::Random
{
static std::mt19937 generator;

export void init(unsigned int seed)
{
    generator.seed(seed);
}

export float range(float min, float max)
{
    std::uniform_real_distribution distribution(min, max);
    return distribution(generator);
}
} // namespace th::Random