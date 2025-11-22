//
// Created by ss on 2025/11/22.
//
module;
#include <stb/stb_image.h>

export module stb_image;

export namespace stbi
{
    auto load = stbi_load;
auto failure_reason = stbi_failure_reason;
auto image_free = stbi_image_free;
}
