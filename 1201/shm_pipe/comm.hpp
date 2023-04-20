
#pragma once

#include <iostream>
#include <cstdio>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <cassert>
#include <string>
#include "Log.hpp"
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// 这些都可以随便写，只要保证我们这个路径是可以有访问权限的就行了
#define PATH_NAME "/home/yufc"
#define PROJ_ID 0x66
#define SHM_SIZE 4096 // 共享内存的大小，最好是页（PAGE: 4096）的整数倍

#define FIFO_NAME "./fifo"

class Init
{
public:
    Init()
    {
        umask(0);
        int n = mkfifo(FIFO_NAME, 0666);
        assert(n == 0);
        (void)n;
        Log("create fifo success", Notice) << std::endl;
    }
    ~Init()
    {
        unlink(FIFO_NAME);
        Log("rm fifo success", Notice) << std::endl;
    }
};


#define READ O_RDONLY
#define WRITE O_WRONLY
int OpenFIFO(std::string pathname, int flags)
{
    int fd = open(pathname.c_str(), flags);
    assert(fd > 0);
    return fd;
}

void Wait(int fd)
{
    Log("等待中...",Notice) << std::endl;
    uint32_t temp = 0;
    ssize_t s = read(fd, &temp, sizeof(uint32_t));
    assert(s == sizeof(uint32_t));
    (void)s;
}

void Signal(int fd)
{
    Log("唤醒中...",Notice) << std::endl;
    uint32_t temp = 1;
    ssize_t s = write(fd, &temp, sizeof(uint32_t));
    assert(s == sizeof(uint32_t));
    (void)s;
}

void CloseFIFO(int fd)
{
    close(fd);
}