module;

#include <filesystem>
#include <fstream>
#include <sstream>

module utils.FileManager;
import spdlog;

namespace fs = std::filesystem;

namespace th::FileManager
{
    // 读取文件内容为字符串
    std::string readFile(const fs::path &path)
    {
        // 详细路径诊断
        fs::path nativePath = path;
        nativePath.make_preferred();

        // 检查文件是否存在
        if (!fileExists(nativePath.string()))
        {
            spdlog::critical("文件不存在:{}", path.string());
        }

        std::ifstream file(nativePath);
        if (!file.is_open())
        {
            spdlog::critical("无法打开文件:{}", nativePath.string());
        }

        // 读取文件内容
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    // 检查文件是否存在
    bool fileExists(const fs::path &path)
    {
        return fs::exists(path);
    }

    fs::path getResourcePath(const fs::path &relativePath, bool isAssets)
    {
        static fs::path basePath;
        if (isAssets)
        {
            basePath = "../../Touhou1-assets/assets";
            return basePath / relativePath;
        }
        // 获取可执行文件路径
        basePath = "../resources";
        auto finalPath = basePath / relativePath;
        if (!fs::exists(finalPath))
        {
            spdlog::error("文件不存在:{}", finalPath.string());
        }

        return basePath / relativePath;
    }
}
