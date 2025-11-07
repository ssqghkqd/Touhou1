#include "utils/Random.hpp"

#include <ctime>
#include <random>

namespace th::Random
{
    std::mt19937 generator;
    
    void init()
    {
        // 使用时间作为种子
        generator.seed(static_cast<unsigned int>(std::time(nullptr)));
    }
    
    float range(float min, float max)
    {
        std::uniform_real_distribution distribution(min, max);
        return distribution(generator);
    }
}