

#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>
#include <stdio.h>
using namespace std;

void *threadRun(void *args)
{
    const string name = (char *)args;
    while (true)
    {
        cout << name << ", pid: " << getpid() << endl;
        // 线程在进程内部运行，所以getpid()的结果应该就是对应父进程的pid
        sleep(1);
    }
}
int main()
{
    pthread_t tid[5];
    char name[64];
    for (int i = 0; i < 5; i++)
    {
        sizeof(int);
        snprintf(name, sizeof name, "%s-%d", "thread", i);  
        pthread_create(tid + i, nullptr, threadRun, (void *)name);
        sleep(1);//缓解传参的bug问题
    }

    while (true)
    {
        cout << "main thread, pid: " << getpid() << endl;
        sleep(3);
    }
    return 0;
}