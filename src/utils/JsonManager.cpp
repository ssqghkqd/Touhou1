#include <utils/FileManager.hpp>
#include <utils/JsonManager.hpp>

#include "utils/Logger.hpp"

namespace th::JsonManager
{
void load(const fs::path& path, const std::string& name)
{
    // 获取该文件的字符串
    const std::string json_str = FileManager::readFile(FileManager::getResourcePath(path));
    const json json_data = json::parse(json_str);
    files[name] = json_data;
}

json get(const std::string& name, const std::string& key)
{
    const json data = files[name];
    if (data.find(key) != data.end())
    {
        return data;
    }
    thLogger::error("该json不存在");
    return json(); // 返回空json
}

void save(const std::string& key, json data)
{
    if (data.find(key) != data.end())
    {
        files[key] = std::move(data);
        return;
    }
    thLogger::error("该json不存在");
}
} // namespace th::JsonManager