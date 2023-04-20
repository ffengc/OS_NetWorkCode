

#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <atomic>
#include <condition_variable>

#if 0
std::mutex mtx;
// tips: 锁是不能当函数参数传递的，因为不支持拷贝
// 加了引用还是报错 -- 就算写了引用，有时候也是拷贝（这个很抽象，但是就是这样）
// 用指针是可以的
void Print(int n)
{
	for (int i = 0; i < n; i++)
	{
		mtx.lock();
		std::cout << std::this_thread::get_id() << " : " << i << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(1));
		mtx.unlock();
	}
}
int main()
{
	std::thread t1(Print, 10);
	std::thread t2(Print, 10);
	t1.join();
	t2.join();

	return 0;
}
#endif

/*template <class Fn, class... Args>
explicit thread (Fn&& fn, Args&&... args);*/
// 这个为什么还是拷贝其实可能和右值引用的底层实现有点关系，这个可以简单验证一下
#if 0
void func(int n, int &x)
{
	x += 10;
}
int main()
{
	int x = 0;
	std::thread t1(func, 10, std::ref(x));
	std::cout << x << std::endl; // 这里还是0
	// 但是加一个ref包起来就能给过去了，我们也不知道是为什么
	t1.join();
	return 0;
}
#endif

/* 所以如果锁不写在全局，就要用ref()包起来传 */

// 传lambda表达式也是一样的
#if 0
int main()
{
	std::mutex mtx;
	int x = 0;
	int n = 10;
	std::thread t1([&]()
				   {
		for(int i = 0; i < n; i++)
		{
			mtx.lock();
			std::cout << std::this_thread::get_id() << " : " << i << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			++x;
			mtx.unlock();
		} });
	std::thread t2([&]()
				   {
		for(int i = 0; i < n; i++)
		{
			mtx.lock();
			std::cout << std::this_thread::get_id() << " : " << i << std::endl;
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			++x;
			mtx.unlock();
		} });
	t1.join();
	t2.join();
	return 0;
}
#endif

// 原子操作相关的
#if 0
int main()
{
	std::mutex mtx;
	int x = 0;
	int n = 10;
	int m = 0;
	std::cin >> m;
	std::vector<std::thread> v(m);
	for (int i = 0; i < m; i++)
	{
		v[i] = std::thread([&]()
						   {
			for(int i = 0;i<n;++i)
			{
				
				mtx.lock(); // 加锁放在for外面，反而更快 --- 为什么？放外面不就是完全是串行了吗？为什么还快了
				// 慢在线程切换了 -- 完全串行只有9次线程切换
				// 并行有很多次
				++x; // 但是如果只有一句++ C++提供了原子操作 让这一句话是原子的
					// CAS 直接由 CPU 支持的 -- 这样就可以实现无锁编程
				std::cout << std::this_thread::get_id() << " : " << x << std::endl;
				mtx.unlock();
			} });
	}
	for (int i = 0; i < m; ++i)
	{
		v[i].join();
	}
	std::cout << x << std::endl; // 注意：这句话放到join前面，结果可能会不一样
	return 0;
}
#endif

// <atomic>
#if 0
int main()
{
	std::mutex mtx;
	std::atomic<int>x(0);
	// int x = 0;
	int n = 10000;
	int m = 0;
	std::cin >> m;
	std::vector<std::thread> v(m);
	for (int i = 0; i < m; i++)
	{
		v[i] = std::thread([&]()
						   {
			for(int i = 0;i<n;++i)
			{
				++x; // 原子的 -- 不加锁也不会有问题
			} });
	}
	for (int i = 0; i < m; ++i)
	{
		v[i].join();
	}
	std::cout << x << std::endl; // 注意：这句话放到join前面，结果可能会不一样
	return 0;
}
#endif
/* 无锁队列: CoolShell 的一个大佬的文章可以看看 */

/* 现在还是有遗留问题 */
// 如果lock和unlock之间抛异常了 -- 死锁
// 所以一般加一个try catch

#if 0
int main()
{
	std::mutex mtx;
	int x = 0;
	int n = 10;
	int m = 0;
	std::cin >> m;
	std::vector<std::thread> v(m);
	for (int i = 0; i < m; i++)
	{
		v[i] = std::thread([&]()
						   {
			for(int i = 0;i<n;++i)
			{
				try
				{
					mtx.lock(); 
					std::cout << std::this_thread::get_id() << " : " << x << std::endl;
					std::this_thread::sleep_for(std::chrono::milliseconds(100));
				}
				catch(...)
				{
					mtx.unlock();
					throw;
				}
				mtx.unlock();
			} });
	}
	for (int i = 0; i < m; ++i)
	{
		v[i].join();
	}
	std::cout << x << std::endl; // 注意：这句话放到join前面，结果可能会不一样
	return 0;
}
#endif

// 但是最好的解决方法还是用智能锁 LockGuard 去搞
// 库里面就有lockGuard了
#if 0
int main()
{
	std::mutex mtx;
	int x = 0;
	int n = 10;
	int m = 0;
	std::cin >> m;
	std::vector<std::thread> v(m);
	for (int i = 0; i < m; i++)
	{
		v[i] = std::thread([&]()
						   {
			for(int i = 0;i<n;++i)
			{
				{
					std::lock_guard<std::mutex> lk(mtx);
					std::cout << std::this_thread::get_id() << " : " << x << std::endl;
				}
				//unique_lock 和 lock_guard 大体是一样的
				//但是unique_lock在RAII上还支持手动 -- 手自一体
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			} });
	}
	for (int i = 0; i < m; ++i)
	{
		v[i].join();
	}
	std::cout << x << std::endl; // 注意：这句话放到join前面，结果可能会不一样
	return 0;
}
#endif

// 曾经有一个题：
// 两个线程交错打印奇数和偶数
#if 0
int main()
{
	int i = 0;
	int n = 100;
	std::thread t1([&]()
				   {
		for(;i<n;)
		{
			//当是偶数的时候，把cpu让出去
			while(i % 2 == 0) // 注意，要用while一直判断
			{
				std::this_thread::yield();
			}
			std::cout << "thread_1: " <<  i <<std::endl;
			i+=1;
		} });
	std::thread t2([&]()
				   {
		for(;i<n;)
		{
			while(i % 2 == 1)
			{
				std::this_thread::yield();
			}
			std::cout << "thread_2: " << i << std::endl;
			i+=1;
		} });
	t1.join();
	t2.join();
	return 0;
}
#endif
/* 注意: 这里的i是线程不安全的 */
// 不用while也能做，给i加锁就行 -- 但是这样是很有隐患的 -- 因为谁去执行只取决于调度 -- 很多偶然因素
// 	可能有一些线程连续运行几次

/* 我们可以用条件变量 -- 不过条件变量是必须配合mutex来用，因为条件变量本身不是线程安全的 */
#if 0
int main()
{
	int i = 0;
	int n = 100;
	std::mutex mtx;
	std::condition_variable cv;
	bool ready = true;
	// 打印奇数
	std::thread t1([&]()
				   {
		for(;i<n;)
		{
			std::unique_lock<std::mutex> lock(mtx);
			cv.wait(lock,[&ready](){return !ready;});
			/* 但是单纯在两个线程加这两句话，不能保证线程2先获取到锁 -- 加上ready变量 */
			std::cout << "thread_1: " <<  i <<std::endl;
			i+=1;
			ready  = true;
			cv.notify_one(); /* 这样才能保证线程2先运行 */
		} });
	// 打印偶数
	std::thread t2([&]()
				   {
		for(;i<n;)
		{
			std::unique_lock<std::mutex> lock(mtx);
			cv.wait(lock,[&ready](){return ready;});
			std::cout << "thread_2: " << i << std::endl;
			i+=1;
			ready  = false;
			cv.notify_one();
		} });
	t1.join();
	t2.join();
	return 0;
}
#endif

// ready 一开始是true
// 如果thread1先抢到锁了 能继续执行吗？不行！因为wait那里阻塞它了，因为wait的第二个参数给它返回false
// 这里很重要，也很巧妙，一定要想清楚

// 所以一开始，无论谁先想到锁，都是线程2开始运行
// 因为线程2运行了一次，ready是true
// 所以第二次进来，无论谁先抢到锁都是线程1运行

/* 利用ready这种思路，我们还可以控制多个线程按顺序运行，ready是1,2,3,4这些就行了 */

/* 讲完这些，我们其中一个结论就是：多线程很难控制！ */