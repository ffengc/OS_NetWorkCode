

#include<unistd.h>
#include<iostream>
#include<vector>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

//进程等待部分代码

typedef void (*handler_t)(); //函数指针类型

std::vector<handler_t> handlers; //函数指针数组

void fun_one()
{
    std::cout<<"这是任务1"<<std::endl;
}
void fun_two()
{
    std::cout<<"这是任务2"<<std::endl;
}
//设置对应的方法回调
void Load()
{
    handlers.push_back(fun_one);
    handlers.push_back(fun_two);
}

int main()
{
    pid_t id = fork();
    if(id == 0)
    {
        //子进程
        int cnt = 5;
        while(cnt--)
        {
            printf("我是子进程: %d\n",cnt);
            sleep(1);
        }
        exit(11);//仅仅用来测试
    }
    else
    {
        //下面的方案是处理非阻塞调用的
        int quit = 0;
        while(!quit)
        {
            int status = 0;
            pid_t res = waitpid(-1,&status,WNOHANG);//以非阻塞方式等待
            if(res > 0)
            {
                //等待成功 -- 子进程退出
                printf("等待子进程退出，退出码: %d\n",WEXITSTATUS(status));
                quit = 1;//让循环break一下
            }
            else if(res == 0)
            {
                //等待成功 -- 但是子进程并未退出
                printf("子进程还在运行中，暂时还没有退出，父进程可以再等一等，处理一下其他事情\n");
                //此时我们父进程做点别的事情呗
                if(handlers.empty())
                {
                    Load();
                }
                for(auto iter : handlers)
                {
                    //执行处理其他任务
                    iter();
                }
            }
            else
            {
                //等待失败
                //其实就是waitpid等待失败，比如我们id写错了，写了1234，但没有1234这个进程
                printf("wait失败!\n");
                quit = 1;//让循环break一下
            }
            sleep(1);
        }
        #if 0
        //下面的方案是处理阻塞调用的！
        //父进程
        int status = 0;
        //注意：只有子进程退出的时候，父进程才会waitpid函数，进行返回！【此时，父进程还活着呢！！！】
        pid_t result = waitpid(id, &status, 0);//默认是在阻塞状态去等待子进程状态变化（就是退出）
        if(result > 0)
        {
            //printf("父进程等待成功，退出码: %d, 退出信号: %d\n",(status>>8)&0xFF,status&0x7F);
            if(WIFEXITED(status))
            {
                printf("子进程执行完毕，子进程的退出码: %d\n",WEXITSTATUS(status));
            }
            else
            {
                printf("子进程异常退出: %d\n",WIFEXITED(status));
            }
        }
        #endif
    }
    return 0;
}