#include "utils/Random.hpp"
#include <ctime>

namespace th
{
    std::mt19937 Random::generator;
    
    void Random::init()
    {
        // 使用时间作为种子
        generator.seed(static_cast<unsigned int>(std::time(nullptr)));
    }
    
    float Random::range(float min, float max)
    {
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(generator);
    }
}