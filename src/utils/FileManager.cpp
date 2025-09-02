#include <filesystem>
#include <fstream>
#include <sstream>
#include <utils/FileManager.hpp>

#include "utils/Logger.hpp"

namespace fs = std::filesystem;

namespace th
{
    // 读取文件内容为字符串
    std::string FileManager::readFile(const fs::path &path)
    {
        // 详细路径诊断
        fs::path nativePath = path;
        nativePath.make_preferred();
        // 详细路径诊断
        thLogger::debug("尝试读取文件: " + nativePath.string());
        thLogger::debug("绝对路径: " + nativePath.string());

        // 检查文件是否存在
        if (!fileExists(nativePath.string()))
        {
            thLogger::critical("File not found: " + nativePath.string());
        }

        std::ifstream file(nativePath);
        if (!file.is_open())
        {
            thLogger::critical("Failed to open file: " + nativePath.string());
        }

        // 读取文件内容
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    // 检查文件是否存在
    bool FileManager::fileExists(const fs::path &path)
    {
        return fs::exists(path);
    }

    fs::path FileManager::getResourcePath(const fs::path &relativePath)
    {
        // 获取可执行文件路径
        static fs::path basePath = "./assets";

        return basePath / relativePath;
    }
}
