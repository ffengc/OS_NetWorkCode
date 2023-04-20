

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <vector>
#include <time.h>
// 要对它进行加锁保护
// pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER; // pthread_mutex_t 原生线程库提供的一个数据类型
int tickets = 10000;

#define THREAD_NUM 5 // 定义线程的个数
struct ThreadData
{
public:
    ThreadData(const std::string n, pthread_mutex_t *pm)
        : tname(n), pmtx(pm) {}

public:
    std::string tname;
    pthread_mutex_t *pmtx;
};

void *getTickets(void *args)
{
    ThreadData *td = (ThreadData *)args;
    while (true)
    {
        pthread_mutex_lock(td->pmtx);
        if (tickets > 0) // 判断的本质也是计算的一种
        {
            usleep(rand() % 1500 + 200); // 让休眠时间随机一点
            printf("%p: %d\n", pthread_self(), tickets);
            tickets--;
            pthread_mutex_unlock(td->pmtx);
        }
        else
        {
            // 没有票了
            pthread_mutex_unlock(td->pmtx);
            break;
        }
    }
    delete td;
}

int main()
{
    pthread_mutex_t mtx;
    pthread_mutex_init(&mtx, nullptr);
    srand((unsigned long)time(nullptr) ^ getpid() ^ 0x147);

    pthread_t t[THREAD_NUM];
    // 多线程抢票的逻辑
    for (int i = 0; i < 5; i++)
    {
        std::string name = "thread ";
        name += std::to_string(i + 1);
        ThreadData *td = new ThreadData(name, &mtx);
        pthread_create(t + i, nullptr, getTickets, (void *)td);
    }
    for (int i = 0; i < THREAD_NUM; i++)
    {
        pthread_join(t[i], nullptr);
    }
    pthread_mutex_destroy(&mtx);
    return 0;
}