

#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

//进程替换部分代码


int main()
{
    printf("当前进程的开始代码!\n");
    //我们想在代码中执行一个名为ls的可执行程序
    // execl("/usr/bin/ls","ls",NULL);
    execl("/usr/bin/lssss","ls","--color=auto","-a","-l",NULL);
    printf("当前进程的结束代码!\n");
    return 0;
}