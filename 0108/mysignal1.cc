

#include <signal.h>
#include <iostream>
#include <unistd.h>
using namespace std;

void showPending(sigset_t *pending)
{
    for (int sig = 1; sig <= 31; sig++)
    {
        if (sigismember(pending, sig))
            cout << "1";
        else
            cout << "0";
    }
    cout << '\n';
}
void handler(int signum)
{
    cout << "获取了一个信号: " << signum << endl;
    int cnt = 10;
    sigset_t pending;
    while (cnt--)
    {
        sigpending(&pending);
        showPending(&pending);
        sleep(1);
    }
}
int main()
{
    // 内核数据类型，但是我们下面这些定义是在user栈上定义的
    struct sigaction act, oact;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_handler = handler;

    //
    sigaddset(&act.sa_mask, 3);
    sigaddset(&act.sa_mask, 4);
    sigaddset(&act.sa_mask, 5);
    sigaddset(&act.sa_mask, 6);
    sigaddset(&act.sa_mask, 7);


    // 把上面定义的东西设置到进程的pcb当中去
    sigaction(2, &act, &oact);
    cout << "default action: " << (int)oact.sa_handler << endl;
    while (true)
        sleep(1);
    return 0;
}