

#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

void handler(int signum)
{
    sleep(1);
    cout << "获得了一个信号: " << signum << endl;
}
int main()
{
    signal(SIGFPE, handler);
    int a = 100;
    a /= 0;
    while (true)
        sleep(1);
    return 0;
}