

#pragma once

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <pthread.h>
#include "threadPool/Log.hpp"
#include "threadPool/threadPool.hpp"
#include "threadPool/Task.hpp"

#define SERVICE_SIZE 1024

static void service(int sock, const std::string &clientIp, const uint16_t &clientPort)
{
    // echo server
    char buffer[SERVICE_SIZE];
    while (true)
    {
        // recvfrom是专门接收udp的
        // 我们用read/write 这两个文件接口可以直接被使用
        ssize_t s = read(sock, buffer, sizeof(buffer) - 1);
        if (s > 0)
        {
            buffer[s] = 0;
            std::cout << clientIp << ": " << clientPort << " # " << buffer << std::endl;
        }
        else if (s == 0) // 对端关闭连接
        {
            // todo
            logMessage(NORMAL, "%s: %d shutdown, me too!", clientIp.c_str(), clientPort);
            break;
        }
        else
        {
            // todo
            logMessage(ERROR, "read socket error, %d: %s", errno, strerror(errno));
            break;
        }
        // 把字符串写回去
        write(sock, buffer, strlen(buffer));
    }
    close(sock);
}

#if false
class ThreadData
{
public:
    int __sock;
    std::string __ip;
    uint16_t __port;
};
#endif

class TcpServer
{
private:
    uint16_t __port;
    std::string __ip;
    int __listen_sock;
    const static int gbacklog = 20; // 一般不能太大也不能太小，后面再详细解释
    std::unique_ptr<ThreadPool<Task>> __threadPool_ptr;

private:
#if false
    static void *__threadRoutine(void *args)
    {
        pthread_detach(pthread_self()); // 线程分离出去，这样主线程就不用管你了，你做完了自己解决问题，主线程不join了
        // 我们想在__threadRoutine里面去调用service函数呢？
        ThreadData *td = static_cast<ThreadData *>(args);
        service(td->__sock, td->__ip, td->__port); // 要调用它，我总得知道参数吧？怎么打包过来？通过ThreadData
        delete td;
        return nullptr;
    }
#endif
public:
    TcpServer(uint16_t port, std::string ip = "")
        : __port(port), __ip(ip), __listen_sock(-1)
        , __threadPool_ptr(ThreadPool<Task>::getThreadPool()) {}
    ~TcpServer() {}
    void initServer()
    {
        // 1. create socket
        __listen_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (__listen_sock < 0)
        {
            logMessage(FATAL, "create socket error, %d: %s", errno, strerror(errno));
            exit(2);
        }
        logMessage(NORMAL, "create socket success, sock: %d", __listen_sock); // 3
        // 2. bind
        struct sockaddr_in local;
        memset(&local, 0, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(__port);
        local.sin_addr.s_addr = __ip.empty() ? INADDR_ANY : inet_addr(__ip.c_str());
        if (bind(__listen_sock, (struct sockaddr *)&local, sizeof(local)) < 0)
        {
            logMessage(FATAL, "bind error, %d: %s", errno, strerror(errno));
            exit(3);
        }
        // 3. 因为tcp是面向连接的，当我们正式通信的时候，需要先建立连接（握手）
        if (listen(__listen_sock, gbacklog) < 0)
        {
            logMessage(FATAL, "listen error, %d: %s", errno, strerror(errno));
            exit(4);
        }
        logMessage(NORMAL, "init TcpServer Success");
    }
    void start()
    {
        // 让线程池去执行run方法
        __threadPool_ptr->run();
        while (true)
        {
            // 4. 获取连接
            struct sockaddr_in src;
            socklen_t len = sizeof(src);
            int service_sock = accept(__listen_sock, (sockaddr *)&src, &len);
            // 它的返回值也是一个套接字
            if (service_sock < 0)
            {
                logMessage(ERROR, "accept error, %d: %s", errno, strerror(errno));
                continue;
                // 获取服务连接失败，并不需要崩掉，而是继续获取！
            }
            // 获取连接成功
            uint16_t client_port = ntohs(src.sin_port);
            std::string client_ip = inet_ntoa(src.sin_addr);
            logMessage(NORMAL, "link success, serviceSock: %d | %s : %d", service_sock, client_ip.c_str(), client_port);
            // 开始进行通信服务
            // version1 -- 单进程循环版
            // service(service_sock, client_ip, client_port);
            // version2 -- 多进程版 -- 忽略SIGCHLD信号
            // version2.1 -- 多进程版本
#if 0
            pid_t id = fork();
            if(id == 0)
            {
                //child
                close(__listen_sock);
                if(fork() > 0) exit(0);
                // 孙子进程 -> os领养 -> os在孤儿进程退出的时候，自动回收孤儿进程
                service(service_sock, client_ip, client_port);
                exit(0);
            }
            //parent
            waitpid(id, nullptr, 0); // 不会阻塞！！
            close(service_sock);
#endif
#if 0
            // version3 -- 多线程版本
            // 把ThreadData准备好
            // 因为是多线程，所以我们尽量不要去用栈上的空间
            // 在多线程这里要不要关闭特定的文件描述符呢？不能关，因为线程是共用文件描述符表的
            ThreadData *td = new ThreadData();
            td->__sock = service_sock;
            td->__ip = client_ip;
            td->__port = client_port;
            pthread_t tid;
            pthread_create(&tid, nullptr, __threadRoutine, td);
#endif
            // version4 -- 线程池版本
            Task t(service_sock, client_ip, client_port, service);
            __threadPool_ptr->pushTask(t);
        }
    }
};