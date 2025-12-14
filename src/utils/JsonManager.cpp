module;
#include <filesystem>
module utils.JsonManager;

import utils.FileManager;
import spdlog;

namespace th::JsonManager
{
void load(const fs::path& path, const std::string& name)
{
    // 获取该文件的字符串
    const std::string jsonStr = FileManager::readFile(FileManager::getResourcePath(path, false));
    const json jsonData = json::parse(jsonStr);
    files[name] = jsonData;
}

json& get(const std::string& name)
{
    if (!files.contains(name))
    {
        spdlog::critical("{} 未加载");
        throw;
    }
    return files[name]; // 返回整个json对象
}

} // namespace th::JsonManager