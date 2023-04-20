
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;
void catchSig(int sig)
{
    // 此时的signum就是捕捉到信号的编号
    std::cout << "进程捕捉到了一个信号,正在处理中: " << sig << " pid: " << getpid() << std::endl;
}
int main()
{
    pid_t id = fork();
    if (id == 0)
    {
        // 子进程
        sleep(1);
        int a = 100;
        a /= 0;
        exit(0);
    }

    int status = 0;
    waitpid(id, &status, 0);
    cout << "父进程: " << getpid() << "子进程: " << id
         << " exit sig: " << (status & 0x7f) << " is core: " << ((status >> 7) & 1) << endl;
    return 0;
}