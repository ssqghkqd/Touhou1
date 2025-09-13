#include <core/App.hpp>
#include <iostream>

int main()
{
    try
    {
        th::App app;
        app.run();
        return 0;
    }
    catch (std::exception &e)
    {
        std::cout << "未捕获异常:" << e.what() << '\n';
        return 1;
    }
}
