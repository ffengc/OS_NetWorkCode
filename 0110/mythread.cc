

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdio>
#include <vector>

// 如果多线程访问同一个全局变量，并对它进行数据计算，多线程会互相影响吗？
int tickets = 10000; // 这里的10000就是临界资源

void *getTickets(void *args)
{
    (void)args;
    while (true)
    {
        if (tickets > 0)
        {
            usleep(1000);
            printf("%p: %d\n", pthread_self(), tickets);
            tickets--;
        }
        else
        {
            // 没有票了
            break;
        }
    }
}
int main()
{
    pthread_t t1, t2, t3;
    // 多线程抢票的逻辑
    pthread_create(&t1, nullptr, getTickets, nullptr);
    pthread_create(&t2, nullptr, getTickets, nullptr);
    pthread_create(&t3, nullptr, getTickets, nullptr);
    pthread_join(t1, nullptr);
    pthread_join(t2, nullptr);
    pthread_join(t3, nullptr);

    return 0;
}