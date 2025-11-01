#include <utils/FileManager.hpp>
#include <utils/JsonManager.hpp>

#include "spdlog/spdlog.h"

namespace th::JsonManager
{
void load(const fs::path& path, const std::string& name)
{
    // 获取该文件的字符串
    const std::string json_str = FileManager::readFile(FileManager::getResourcePath(path, false));
    const json json_data = json::parse(json_str);
    files[name] = json_data;
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