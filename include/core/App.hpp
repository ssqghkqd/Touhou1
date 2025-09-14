#pragma once
#include <../../thirdparty/entt/src/entt/entt.hpp>

#include "entt/entity/registry.hpp"

namespace th
{

    class App
    {
    public:
        App();
        ~App() = default;
        void run();
        void mainLoop();
        void init();
        void update(double& lastPrintTime);


        inline static const int width = 1280;
        inline static const int height = 960;
        inline static const char *title = "Touhou 1 ~ religious war";
        inline static const int bgwidth = 768;
        inline static const int bgheight = 896;
        inline static const int bgoffsetX = 64;
        inline static const int bgoffsetY = 32;
        inline static int scale = 1;

    private:
        entt::registry registry;
    };
} // namespace th
