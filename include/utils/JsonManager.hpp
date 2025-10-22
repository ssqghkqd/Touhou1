#pragma once

#include <filesystem>
#include <nlohmann_json/json.hpp>

namespace fs = std::filesystem;
using json = nlohmann::json;

namespace th::JsonManager
{
// json文件的名字与数据的哈希表
inline std::unordered_map<std::string, json> files;

void load(const fs::path& path, const std::string& name);


json get(const std::string& name, const std::string& key);
void save(const std::string& key, json data);
} // namespace th::JsonManager