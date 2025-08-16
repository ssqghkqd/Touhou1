#pragma once
#include <stdafx.hpp>
#include <filesystem>

namespace fs = std::filesystem;

namespace th
{
    class FileManager
    {
    public:
        static std::string readFile(const fs::path &path);
        static bool fileExists(const fs::path &path);
        static fs::path getResourcePath(const fs::path &relativePath);
    };
}
