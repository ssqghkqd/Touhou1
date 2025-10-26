#pragma once

namespace th
{
class ResourceManager
{
  public:
    static ResourceManager& get();
    void init();

  private:
    ResourceManager() = default;
    ~ResourceManager() = default;
};
} // namespace th
