
#include "comm.hpp"
#include <sys/wait.h>

static void getMessage(int fd)
{   
    char buffer[SIZE];
    while (true)
    {
        memset(buffer, '\0', sizeof(buffer));
        ssize_t s = read(fd, buffer, sizeof(buffer) - 1);
        // 最好不要让缓冲区写满，因为没有\0 \0不算的，写满了\0我们自己没地方放了
        if (s > 0)
        {
            cout << "[" << getpid() << "]"
                 << "client say: > " << buffer << endl;
            // 加上getpid()看看是被哪一个子进程读出来的
        }
        else if (s == 0)
        {
            // end of file
            // 读端读到0，说明写端关闭了
            cerr << "[" << getpid() << "]"
                 << "read end of file, client quit, server quit too!" << endl;
            break;
        }
        else
        {
            // read error
            perror("read");
            break;
        }
    }
}

int main()
{
    // 1.创建管道文件
    if (mkfifo(ipcPath.c_str(), MODE) < 0)
    {
        perror("mkfifo");
        exit(1);
    }
    // 放一些日志
    Log("创建管道文件成功", Debug) << "step 1" << endl;
    // 2.正常的文件操作
    int fd = open(ipcPath.c_str(), O_RDONLY);
    if (fd < 0)
    {
        perror("open");
        exit(2);
    }
    Log("打开管道文件成功", Debug) << "step 2" << endl;

    // 如果我们此时去创建子进程呢？
    int nums = 3;
    for (int i = 0; i < nums; i++)
    {
        pid_t id = fork();
        if (id == 0)
        {
            // 子进程
            // 让每一个子进程都getmessage
            getMessage(fd);
            exit(1);
        }
    }
#if 0
    // 3.便携正常的通信代码了
    // 读文件
    char buffer[SIZE];
    while (true)
    {
        memset(buffer, '\0', sizeof(buffer));
        ssize_t s = read(fd, buffer, sizeof(buffer) - 1);
        // 最好不要让缓冲区写满，因为没有\0 \0不算的，写满了\0我们自己没地方放了
        if (s > 0)
        {
            cout << "client say: > " << buffer << endl;
        }
        else if (s == 0)
        {
            // end of file
            // 读端读到0，说明写端关闭了
            cerr << "read end of file, client quit, server quit too!" << endl;
            break;
        }
        else
        {
            // read error
            perror("read");
            break;
        }
    }
    // 4.关闭文件
    close(fd);
    Log("关闭管道文件成功", Debug) << "step 3" << endl;
    unlink(ipcPath.c_str()); // 通信完毕就删除文件
    Log("删除管道文件成功", Debug) << "step 4" << endl;
#endif
    // 此时父进程干嘛呢？等待！
    for (int i = 0; i < nums; i++)
    {
        waitpid(-1, nullptr, 0);
    }
    return 0;
}