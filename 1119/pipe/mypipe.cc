#include <iostream>
#include <unistd.h>
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;
int main()
{
    // 1.创建管道
    int pipefd[2] = {0}; // pipefd[0]:读端  pipefd[1]: 写端
    int n = pipe(pipefd);
    assert(n != -1); // debug assert 是有效的, release assert 是无效的
    // 此时会因为n没有被使用过从而大量报警
    (void)n; // 仅仅表明n被使用过

#if DEBUG
    // 此时我们看看 pipefd[]里面存的文件描述符都是些啥东西
    cout << "pipefd[0]: " << pipefd[0] << endl; // 3
    cout << "pipefd[1]: " << pipefd[1] << endl; // 4
#endif
    // 2.创建子进程
    pid_t id = fork();
    assert(id != -1);
    if (id == 0)
    {
        // 子进程
        // 3.构建单向通信的信道 --- 父进程写入，子进程读取
        // 3.1 关闭子进程不需要的fd
        close(pipefd[1]); // 把写端关闭

        // 通信
        char buffer[1024]; // 需要一个缓冲区来读

        //写入的一端：如果fd没有关闭，读端如果有数据就读，如果没有数据，就等！
        //写入的一端：如果fd关闭了，读取的一端read会返回0，表示读到了文件的结尾！
        while (true)
        {
            ssize_t s = read(pipefd[0], buffer, sizeof(buffer) - 1);
            if (s > 0)
            {
                // 说明读成功了
                buffer[s] = 0;
                cout << "child get a message [" << getpid() << "]"
                     << " Father# " << buffer << endl;
            }
            else if(s == 0)
            {
                //read返回的是0
                cout<<"writer quit(father),me(child) quit!"<<endl;
                break;
            }
        }

        exit(0);
    }
    // 父进程
    // 3.构建单向通信的信道
    close(pipefd[0]);
    // 写入
    string message = "我是父进程，我正在给你发消息";
    int count = 0; // 记录父进程发送信息的条数目
    char send_buffer[1024];
    while (true)
    {
        // 3.2 构建一个变化的字符串
        snprintf(send_buffer, sizeof(send_buffer), "%s[%d] : %d", message.c_str(), getpid(), count);
        // 3.3 发送
        write(pipefd[1], send_buffer, strlen(send_buffer)); // 不需要+1
        count++;
        // 3.4 故意sleep一次
        sleep(1);
        if(count == 10) //如果写入的一端退出了，读的那一端端read就会返回0
        {
            cout<<"writer quit(father)"<<endl;
            break;
        }
    }
    close(pipefd[1]);//退出之后关闭我们的文件描述符
    pid_t ret = waitpid(id, nullptr, 0);
    assert(ret > 0);
    (void)ret;
    return 0;
}