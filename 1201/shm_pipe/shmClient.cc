
#include "comm.hpp"

int main()
{
    Log("child pid is: ", Debug) << getpid() << std::endl;
    key_t k = ftok(PATH_NAME, PROJ_ID);
    if (k < 0)
    {
        Log("create key failed", Error) << " client key: " << k << std::endl;
        exit(1);
    }
    Log("create key done", Debug) << " client key: " << k << std::endl;
    // 获取共享内存
    int shmid = shmget(k, SHM_SIZE, IPC_CREAT);
    if (shmid < 0)
    {
        Log("create shm success", Error) << " client key: " << k << std::endl;
        exit(2);
    }
    Log("create shm failed", Debug) << " client key: " << k << std::endl;
    // sleep(5);
    //
    char *shmaddr = (char *)shmat(shmid, nullptr, 0);
    if (shmaddr == nullptr)
    {
        Log("attach shm failed", Error) << " client key: " << k << std::endl;
        exit(3);
    }
    Log("attach shm success", Debug) << " client key: " << k << std::endl;
    // sleep(5);

    // 通信控制
    int fd = OpenFIFO(FIFO_NAME, WRITE);
    // 使用
    while (1)
    {
        ssize_t s = read(0, shmaddr, SHM_SIZE - 1); // 直接从键盘读取，读取完直接写到共享内存里面
        if (s > 0)
        {
            shmaddr[s - 1] = 0;
            Signal(fd); // 唤醒对方
            if (strcmp(shmaddr, "quit") == 0)
                break;
        }
    }
    CloseFIFO(fd);
    // 将共享内存看作一个char类型的buffer
    // char a = 'a';
    // for (; a <= 'c'; a++)
    // {
    //     // 每一次都向shmaddr[共享内存的起始地址]写入
    //     snprintf(shmaddr, SHM_SIZE - 1, "hello server, 我是其他进程, 我的pid: %d, inc: %c", getpid(), a);
    //     sleep(2);
    // }
    // 写完之后，写一个quit
    // strcpy(shmaddr, "quit");

    // 去关联
    int n = shmdt(shmaddr);
    assert(n != -1);
    Log("detach shm success", Debug) << " client key: " << k << std::endl;
    // sleep(5);

    // client要不要chmctl删除呢？不需要！！！

    return 0;
}