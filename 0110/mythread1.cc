

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <vector>
using namespace std;

//__thread: 修饰全局变量，带来的结果就是让每一个线程独自拥有一个全局的变量 --- 线程的局部存储！
__thread int g_val = 0;

void *threadRoutine(void *args)
{
    sleep(5);
    execl("/usr/bin/ls", nullptr);
    while (true)
    {
        cout << "   " << (char *)args << ": " << g_val << " &g_val: " << &g_val << endl;
        g_val++;
        sleep(1);
    }
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, threadRoutine, (void *)"thread 1");
    printf("%lu, %p\n", tid, tid);

    while (true)
    {
        sleep(1);
        cout << "main thread: " << g_val << " &g_val: " << &g_val << endl;
    }

    return 0;
}