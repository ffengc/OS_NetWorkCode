

#include <iostream>
#include <signal.h>
#include <unistd.h>

void catchSig(int sig)
{
    // 此时的signum就是捕捉到信号的编号
    std::cout << "进程捕捉到了一个信号,正在处理中: " << sig << " pid: " << getpid() << std::endl;
}
int main()
{
    // signal(SIGINT, catchSig);//特定信号的处理动作，一般只有一个，所有捕捉到了，进程不退出了
    // signal(SIGQUIT, catchSig);//3号信号是有coredump的
    while (true)
    {
        std::cout << "我是一个进程，我正在运行..., pid: " << getpid() << std::endl;
        int a = 0;
        int b = 4/a; //除0错误
        // sleep(1);
        std::cout<<"run here ... "<<std::endl;
    }
    return 0;
}