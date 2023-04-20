

#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>

#define TNUM 4

typedef void (*func_t)(const std::string &name, pthread_mutex_t *pmtx, pthread_cond_t *pcond);
volatile bool quit = false;


class ThreadData
{
public:
    std::string __name;
    func_t __func;
    pthread_mutex_t *__pmtx;
    pthread_cond_t *__pcond;

public:
    ThreadData(const std::string &name, func_t func, pthread_mutex_t *pmtx, pthread_cond_t *pcond)
        : __name(name), __func(func), __pmtx(pmtx), __pcond(pcond) {}
};

// ==================== 线程要执行的函数 ==================== //
#if 1
void func1(const std::string &name, pthread_mutex_t *pmtx, pthread_cond_t *pcond)
{
    while (!quit) // 当quit = true之后，所有线程退出
    {
        //wait一定在加锁和解锁之间！
        pthread_mutex_lock(pmtx);
        //这里要做的一件事其实就是:
        //if(临界资源就绪吗？不就绪)
        //  刚开始学的做法是：break，然后重新检测
        //  但是现在我不想让线程一直检测了，那么就让它去等！等到就绪为止
        //所以！其实我们访问了临界资源！
        pthread_cond_wait(pcond, pmtx); //默认该线程再执行的时候，wait代码被执行，当前线程会立即被阻塞！
        std::cout << name << " A_running..." << std::endl;
        // sleep(1);
        pthread_mutex_unlock(pmtx);
    }
}
void func2(const std::string &name, pthread_mutex_t *pmtx, pthread_cond_t *pcond)
{
    while (!quit)
    {
        pthread_mutex_lock(pmtx);
        pthread_cond_wait(pcond, pmtx); //默认该线程再执行的时候，wait代码被执行，当前线程会立即被阻塞！
        std::cout << name << " B_running..." << std::endl;
        // sleep(1);
        pthread_mutex_unlock(pmtx);
    }
}
void func3(const std::string &name, pthread_mutex_t *pmtx, pthread_cond_t *pcond)
{
    while (!quit)
    {
        pthread_mutex_lock(pmtx);
        pthread_cond_wait(pcond, pmtx); //默认该线程再执行的时候，wait代码被执行，当前线程会立即被阻塞！
        std::cout << name << " C_running..." << std::endl;
        // sleep(1);
        pthread_mutex_unlock(pmtx);
    }
}
void func4(const std::string &name, pthread_mutex_t *pmtx, pthread_cond_t *pcond)
{
    while (!quit)
    {
        pthread_mutex_lock(pmtx);
        pthread_cond_wait(pcond, pmtx); //默认该线程再执行的时候，wait代码被执行，当前线程会立即被阻塞！
        std::cout << name << " D_running..." << std::endl;
        // sleep(1);
        pthread_mutex_unlock(pmtx);
    }
}
#endif
// ==================== 线程要执行的函数 ==================== //
void *Entry(void *args)
{
    ThreadData *td = (ThreadData *)args;             // td再每一个线程自己私有的栈空间中保存
    td->__func(td->__name, td->__pmtx, td->__pcond); // 它是一个函数，调用完成就要返回！
    delete td;
    return nullptr;
}

int main()
{
    // 我们构建好多线程运行架构之后
    // 我们要开始控制这些线程了！
    // 先定义互斥锁和条件变量
    pthread_mutex_t mtx;
    pthread_cond_t cond;
    // 初始化
    pthread_mutex_init(&mtx, nullptr);
    pthread_cond_init(&cond, nullptr);

    pthread_t tids[TNUM];
// 让不同的线程做不同的工作
#if 1
    func_t funcs[TNUM] = {func1, func2, func3, func4};
#endif
    for (int i = 0; i < TNUM; i++)
    {
        // 让不同的线程做不同的工作
        std::string name = "Thread ";
        name += std::to_string(i + 1);
        ThreadData *td = new ThreadData(name, funcs[i], &mtx, &cond);
        pthread_create(tids + i, nullptr, Entry, (void *)td);
    }
    std::cout << "new threads generate success" << std::endl;
    std::cout << "main thread begin to control all new threads ..." << std::endl;
    //控制对应的线程按照一定规则执行

    int cnt = 10; //控制10次之后退出
    while(cnt--)
    {
        pthread_cond_signal(&cond);
        sleep(1);
    }
    std::cout << "threads ctrl done!" << std::endl;
    quit = true;
    //因为改成true之后，线程的循环就进不去了，所有线程可能都在等着
    //所以这里再唤醒最后一次，让线程退出
    pthread_cond_broadcast(&cond);

    //当cnt倒数完成，我们应该就能看到join的信息
    for (int i = 0; i < TNUM; i++)
    {
        pthread_join(tids[i], nullptr);
        std::cout << "thread: " << tids[i] << " quit" << std::endl;
    }

    pthread_mutex_destroy(&mtx);
    pthread_cond_destroy(&cond);
    return 0;
}