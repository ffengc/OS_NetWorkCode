


#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <atomic>
#include <condition_variable>

/* ---------------------------------- shared_ptr ---------------------------------- */

// shared_ptr是线程安全的吗
// bit::shared_ptr 不是线程安全的

// shared_ptr安全吗？
// 应该有两个角度：
// 它的引用计数是安全的，它所指向/访问的资源不是安全的

// STL是不是线程安全的？
// 不是的

/* ------------------------------------ 单例模式 ------------------------------------ */
// 饿汉模式：
// 1. 不允许随便创建对象，构造函数私有
// 2. 防拷贝
// 3. main函数之前就创建初始化对象
// 全局只有一个唯一对象
// 缺点：1. 如果对象初始化麻烦，影响程序启动速度 2. 如果有多个单例类，如果有依赖顺序关系，无法控制

// 懒汉模式：
// 1. 不允许随便创建对象，构造函数私有
// 2. 防拷贝
// 3. 第一次使用对象时创建对象
// 新问题：如果多个线程使用，第一次调用的时候存在竞争的线程安全问题


class Singleton
{
public:
    static Singleton* GetInstance()
    {
        if(__pInstense == nullptr) /* 这一层检查的目的是：保证对象创建好之后，不用获取锁，提高效率 */
        {
            //第一次
            std::unique_lock<std::mutex> lock(__mtx);
            /* 这个叫做双检查加锁 -- 蛋哥也讲过 */
            if(__pInstense == nullptr) /* 这一层检查的目的是：保证第一次访问时，线程安全 */
            {
                __pInstense = new Singleton; // 两个线程同时进来怎么办？
            }
            //加锁写在这里可以解决问题吗？不可以
            //这样只能解决后面进来的问题，第一次的问题还没有解决
        }
        return __pInstense;
    }
private:
    Singleton(){}
    Singleton(Singleton const&) = delete;
    Singleton& operator=(Singleton const&) = delete;
    static Singleton* __pInstense;
    static std::mutex __mtx;
};
Singleton* Singleton::__pInstense = nullptr; // 在程序入口之前就完成单例对象的初始化
std::mutex Singleton::__mtx;

int main()
{
    Singleton* ptr1 = Singleton::GetInstance();
    Singleton* ptr2 = Singleton::GetInstance();
    std::cout << ptr1 << " " << ptr2 << std::endl;
    return 0;
}

// 如果担心编译器重排指令 -- 可以加一个内存栅栏 -- 具体是什么可以去查
// 平时写是不需要考虑这个问题的 -- 因为编译器一般不乱搞是不会乱优化的