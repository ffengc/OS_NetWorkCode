

#pragma once

#include <iostream>
#include <functional>

// static void service(int sock, const std::string &clientIp, const uint16_t &clientPort)
using func_t = std::function<void(int, const std::string &, const uint16_t &)>;

class Task
{
public:
    int __sock;
    std::string __ip;
    uint16_t __port;
    func_t __func;

public:
    Task() {}
    Task(int sock, const std::string ip, uint16_t port, func_t func)
        : __sock(sock), __ip(ip), __port(port), __func(func)
    {
    }
    void operator()(const std::string &name)
    {
        __func(__sock, __ip, __port);
    }
};