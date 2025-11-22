//
// Created by ss on 2025/11/22.
//
module;
#include <filesystem>
#include <unordered_map>

export module utils.JsonManager;
import nlohmann_json;

namespace fs = std::filesystem;
using json = nlohmann::json;

export namespace th::JsonManager
{
// json文件的名字与数据的哈希表
inline std::unordered_map<std::string, json> files;

void load(const fs::path& path, const std::string& name);

json& get(const std::string& name);
} // namespace th::JsonManager
