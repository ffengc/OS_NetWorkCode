

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <vector>
using namespace std;

void *threadRoutine(void *args)
{
    pthread_detach(pthread_self());
    int cnt = 0;
    while (cnt--)
    {
        cout << (char *)args << " id: " << pthread_self() << endl;
        sleep(1);
    }
    pthread_exit((void *)11);
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, threadRoutine, (void *)"thread 1");
    printf("%lu, %p\n", tid, tid);

    while (true)
    {
        sleep(1);
        cout << "main thread"
             << " id: " << pthread_self() << endl;
        sleep(1);
        break;
    }

    return 0;
}