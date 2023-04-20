

#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <assert.h>

// test1
#if 0
void catchSig(int signum)
{
    std::cout << "获得了一个信号: " << signum << std::endl;
}
int main()
{
    // 处理信号
    // signal(信号编号,方法)
#if 0
    signal(2,SIG_IGN);
    signal(2,SIG_DFL);
#endif
    for (int i = 0; i <= 31; i++)
    {
        signal(i, catchSig);
    }
    while (true)
    {
        std::cout << "pid: " << getpid() << std::endl;
        sleep(1);
    }
    return 0;
}
#endif

// test2
#if 0
static void showPending(sigset_t &pending)
{
    for (int sig = 1; sig <= 31; sig++)
    {
        if (sigismember(&pending, sig))
        {
            std::cout << "1";
        }
        else
            std::cout << "0";
    }
    std::cout << std::endl;
}
void catchSig(int signum)
{
    std::cout << "获得了一个信号: " << signum << std::endl;
}
int main()
{
    // 0. 为了方便测试，捕捉2号信号
    signal(2, catchSig);
    // 1. 定义信号集对象
    sigset_t bset, obset;
    sigset_t pending;
    // 2. 初始化
    sigemptyset(&bset);
    sigemptyset(&obset);
    sigemptyset(&pending);
    // 3. 添加我们要进行屏蔽的信号
    sigaddset(&bset, 2);
    // 4. 设置set到内核对应的进程当中 [默认情况：进程不会对任何信号进行block]
    int n = sigprocmask(SIG_BLOCK, &bset, &obset);
    assert(!n);
    (void)n;
    std::cout << "block 2号信号成功..." << std::endl;
    // 5. 重复打印当前进程的pending信号集
    int count = 0;
    while (true)
    {
        // 获取当前进程的pending信号集
        sigpending(&pending);
        // 现实pending信号集中没有被递达的信号
        showPending(pending);
        sleep(1);
        count++;
        if (count == 10)
        {
            // 恢复
            int n = sigprocmask(SIG_SETMASK, &obset, nullptr);
            //因为前面我们得到了老的block，即obset
            //我们直接用obset去设置就行了，用SIG_SETMASK选项
            //当然，用SIG_UNBLOCK也是可以的

            //默认情况下，恢复了2号信号以后，2号信号就递达了
            //而2号信号的默认处理动作是终止进程！
            //所以如果我们想看现象，就捕捉一下二号信号就行
            assert(n == 0);
            (void)n;
            std::cout << "解除对于2号信号的block" << std::endl;
        }
    }
    return 0;
}
#endif

// test3
static void showPending(sigset_t &pending)
{
    for (int sig = 1; sig <= 31; sig++)
    {
        if (sigismember(&pending, sig))
        {
            std::cout << "1";
        }
        else
            std::cout << "0";
    }
    std::cout << std::endl;
}
static void blockSig(int sig)
{
    // 这个接口的作用就是屏蔽sig信号
    sigset_t bset;
    sigemptyset(&bset);
    sigaddset(&bset, sig);
    int n = sigprocmask(SIG_BLOCK, &bset, nullptr);
    assert(!n);
    (void)n;
}
int main()
{
    // blockSig(2);
    for (int sig = 1; sig <= 32; sig++)
    {
        blockSig(sig);
    }
    sigset_t pending;
    while(true)
    {
        sigpending(&pending);
        showPending(pending);
        sleep(1);
    }
    return 0;
}