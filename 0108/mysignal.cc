

#include <signal.h>
#include <iostream>
#include <unistd.h>
using namespace std;

#if 0
volatile int flag = 0;
void changeFlag(int signum)
{
    (void)signum;
    cout << "change flag: " << flag;
    flag = 1;
    cout << "->" << flag << endl;
}
int main()
{
    signal(2, changeFlag);
    while (!flag)
    {
        ;
    }
    cout << "进程正常退出后: " << flag << endl;
    return 0;
}
#endif

// SIGCHLD

// 1
#if 0
void handler(int signum)
{
    cout << "子进程退出: " << signum << endl;
    //如果现在有10个子进程都要退出
    //while(wait()) ...
    //如果10个子进程有5个要退出
    
}
int main()
{
    signal(SIGCHLD, handler);
    if (fork() == 0)
    {
        sleep(1);
        exit(0);
    }
    while (true)
        sleep(1);
    return 0;
}
#endif

// 2
// 如果我们不想等待子进程
// 还想让子进程退出之后自动释放
int main()
{
    signal(SIGCHLD, SIG_IGN);//手动设置子进程忽略
    if (fork() == 0)
    {
        cout << "child: " << getpid() << endl;
        sleep(5);
        exit(0);
    }

    while (true)
    {
        cout << "parent: " << getpid() << " 执行我自己的任务!" << endl;
        sleep(1);
    }
    return 0;
}