// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (C) 2025 ss
// 文件工具
module;
#include <expected>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

export module utils:FileUtils;
import defs;
import spdlog;

namespace fs = std::filesystem;

export namespace th::FileUtils
{
// 读取文件内容为字符串 必须保证文件已存在（因为先使用获取完整路径函数）
std::expected<std::string, std::string> readFileToString(const fs::path& path)
{
    fs::path nativePath = path;
    nativePath.make_preferred();

    std::ifstream file(nativePath);
    if (!file.is_open())
    {
        spdlog::error("无法打开文件:{}", nativePath.string());
        return std::unexpected("无法打开文件");
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool isExist(const fs::path& path)
{
    return std::filesystem::exists(path);
}

std::expected<std::vector<char>, err> readFileToBytes(const fs::path& path)
{
    std::ifstream file(path, std::ios::ate | std::ios::binary);
    if (!file.is_open())
    {
        spdlog::error("无法打开文件:{}", path.string());
        return std::unexpected(err::file_open_failed);
    }

    const size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);
    file.seekg(0); // 回到开头
    file.read(buffer.data(), fileSize);

    file.close();
    return buffer;
}

std::expected<fs::path, err> getResourcePath(const fs::path& relativePath, const bool isAssets)
{
    // TODO 目前硬编码了路径
    static fs::path basePath;
    basePath = "../resources";
    if (isAssets)
    {
        basePath = "../../Touhou1-assets/assets";
    }
    const auto finalPath = basePath / relativePath;
    if (!fs::exists(finalPath))
    {
        spdlog::error("文件不存在:{}", finalPath.string());
        return std::unexpected(err::file_not_found);
    }

    return basePath / relativePath;
}
} // namespace th::FileUtils