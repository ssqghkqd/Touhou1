#pragma once

#include "entt/entt.hpp"
namespace th
{
    class Context
    {
    public:
        // 删除拷贝构造和赋值操作符
        Context(const Context &) = delete;
        Context &operator=(const Context &) = delete;

        // 移动语义也删除（单例不需要）
        Context(Context &&) = delete;
        Context &operator=(Context &&) = delete;

        ~Context();
        void init();

        static Context &getInstance();

        entt::registry &getRegistry();
        entt::entity createPlayer(entt::registry &registry);

        inline bool getDebug() const { return openDebug; }
        inline void toggleDebug() { openDebug = !openDebug; }

    private:
        Context() = default;
        bool contextInitialized = false;
        entt::registry registry;

        bool openDebug = false;
    };
} // namespace th
