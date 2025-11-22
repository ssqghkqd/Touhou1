//
// Created by ss on 2025/11/22.
//
module;
#include <filesystem>
export module utils.FileManager;

export namespace th::FileManager
{
std::string readFile(const std::filesystem::path& path);
bool fileExists(const std::filesystem::path& path);
std::filesystem::path getResourcePath(const std::filesystem::path& relativePath, bool isAssets = false);
} // namespace th::FileManager
