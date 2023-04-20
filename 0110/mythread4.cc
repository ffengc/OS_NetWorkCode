

#include <iostream>
#include <algorithm>
#include <unistd.h>
#include <thread>

void fun()
{
    while(true)
    {
        std::cout << "hello C++ new thread" << std::endl;
        sleep(2);
    }
}
int main()
{
    std::thread t(fun);
    while(true)
    {
        std::cout << "hello C++ main thread" << std::endl;
        sleep(1);
    }
    t.join();
    return 0;
}