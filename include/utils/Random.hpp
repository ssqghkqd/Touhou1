#pragma once
#include <random>

namespace th
{
    class Random
    {
    public:
        static void init();
        static float range(float min, float max);
        
    private:
        static std::mt19937 generator;
    };
}