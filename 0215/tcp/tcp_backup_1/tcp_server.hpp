

#pragma once

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include "Log.hpp"

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
}

class TcpServer
{
private:
    uint16_t __port;
    std::string __ip;
    int __listen_sock;
    const static int gbacklog = 20; // 一般不能太大也不能太小，后面再详细解释
public:
    TcpServer(uint16_t port, std::string ip = "")
        : __port(port), __ip(ip), __listen_sock(-1) {}
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
        signal(SIGCHLD, SIG_IGN); // 对于SIGCHLD，主动忽略SIGCHLD信号，子进程退出之后
                                  // 会自动释放，父进程等都不用等
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
            // version2 -- 多进程版
            // 创建子进程 -- 让子进程给新的连接提供服务，子进程能不能打开父进程曾经打开的fd呢？
            pid_t id = fork();
            if(id == 0)
            {
                // 子进程 -- 我们可以让子进程进行读取，父进程继续提供上面的服务
                // 但是如果子进程处理完之后，退出后，会进入僵尸状态

                // 第二个问题：
                // 子进程会继承父进程打开的文件和文件fd
                // 现在的问题是，子进程需不需要知道监听sock呢？不需要！
                // 所以子进程关闭自己不需要的fd
                close(__listen_sock);
                service(service_sock, client_ip, client_port); // 提供服务
                exit(0);
            }
            // 父进程
            // waitpid(); // 阻塞式等待
            // 父进程需不需要知道服务sock呢？不需要！
            close(service_sock);
        }
    }
};