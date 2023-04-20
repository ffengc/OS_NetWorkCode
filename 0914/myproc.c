

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

#if 0
int main()
{
    for(int number = 0;number<150;number++)
    {
        printf("%d: %s\n",number,strerror(number));
    }
    return 0;
}
#endif

#if 0
int main()
{
    printf("hello world1\n");
    printf("hello world1\n");
    printf("hello world1\n");
    printf("hello world1\n");
    int *p = NULL;
    *p = 1234;//野指针
    printf("hello world2\n");
    printf("hello world2\n");
    printf("hello world2\n");
    printf("hello world2\n");
    return 0;
}
#endif


#if 0
int main()
{
    printf("你可以看见我吗？");
    sleep(3);
    _exit(13);
    return 0;
}
#endif


//进程等待
#include<sys/types.h>
#include<sys/wait.h>
int main()
{
    pid_t id = fork();
    if(id < 0)
    {
        perror("fork");
        exit(1);//标识继承运行完毕，结果不正确
    }
    else if(id == 0)
    {
        //子进程
        int cnt = 5;
        while(cnt)
        {
            printf("cnt: %d, 我是子进程, pid: %d,ppid: %d\n",cnt,getpid(),getppid());
            sleep(1);
            //cnt--;不让他自动退出

            // int a = 10;
            // a /= 0;//我们弄一个错误出来
            // int *p = NULL;
            // *p = 100;

            //此时我不让它退出了

        }
        exit(105);//给父进程返回105
    }
    else
    {
        //父进程
        //此时我们不用死循环等待了 -- 这样太蠢了
        printf("我是父进程, pid: %d,ppid: %d\n",getpid(),getppid());
        int status = 0;//等下这里要得到子进程返回回来的码
        pid_t ret = waitpid(id,&status,0);//注意 -- 这里是阻塞式的等待
        if(ret > 0)
        {
            printf("等待子进程成功, ret: %d, status: %d, 子进程收到的信号编号: %d, 子进程的退出码: %d\n",
                ret,status,status&0x7F,(status>>8)&0xFF);
        }

    }
    return 0;
}