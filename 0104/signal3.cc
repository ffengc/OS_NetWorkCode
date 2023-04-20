

#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

#if 0
int main()
{
    std::cout << "我正在运行" << std::endl;
    sleep(1);
    abort();
    return 0;
}
#endif

//定时器功能
int count = 0;
void catchSig(int sig)
{
    // 此时的signum就是捕捉到信号的编号
    std::cout << "进程捕捉到了一个信号,正在处理中: " << sig << " pid: " << getpid() << std::endl;
    std::cout << "count: " << count << std::endl;
    //如果我们想重复设置闹钟呢？
    alarm(1);//这里重新定义闹钟就行了
}
int main()
{
    // 验证1s之内，我们一共会进行多少次count++
    // 1. 为什么我们只计算到1w+左右呢？主要是因为用了cout + 网络发送 = IO
    // 2. 如果单传想要计算算力呢？
    alarm(1); //一旦触发，就自动移除了
    signal(SIGALRM, catchSig);

    while (true)
    {
        count++;
    }
    return 0;
}