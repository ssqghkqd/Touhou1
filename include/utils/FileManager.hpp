#pragma once
#include <filesystem>

namespace th::FileManager
{
std::string readFile(const std::filesystem::path& path);
bool fileExists(const std::filesystem::path& path);
std::filesystem::path getResourcePath(const std::filesystem::path& relativePath, bool isAssets = false);
} // namespace th::FileManager
