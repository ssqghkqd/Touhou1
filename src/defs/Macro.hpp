// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
// 错误处理宏

#pragma once

// e是operr
#define CHECK(e)           \
    do                     \
    {                      \
        if (e.has_value()) \
        {                  \
            return e;      \
        }                  \
    }                      \
    while (0)