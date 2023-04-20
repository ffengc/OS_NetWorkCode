


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>


// shell运行原理: 通过让子进程执行命令，父进程等待和解析命令
// 为什么要子进程呢 -- 这样就算命令执行在子进程里面出错 -- 也不会影响父进程
// 因此我们不怕命令在命令行中出错
//


#define NUM 1024 // 定义一个大小为1024的缓冲区
char cmd_line[NUM]; //保存完整的命令行字符串
#define SIZE 32
char *g_argv[SIZE]; //保存拆分之后的命令行字符串

#define SEP " " //定义命令分隔符

int main()
{
    //获取全局的环境变量
    extern char** environ;


    //0. 命令行解释器，一定是一个常驻内存的进程 -- 不退出
    while(1)
    {
        //1. 打印出提示信息 [yufc@localhost myshell]# 
        //  其实这一串是可以用系统接口来获取的 -- 不过我们不关心这些
        printf("[yufc@localhost myshell]# ");
        fflush(stdout);//手动刷新
        
        //2. 获取用户的键盘输入[输入指的是各种指令和选项]
        // 要输入 -- 我们就要提供一个输入的缓冲区
        memset(cmd_line,'\0',sizeof cmd_line); //sizeof不是函数 -- 是运算符, 所以可以不用()
        if(fgets(cmd_line,sizeof cmd_line, stdin) == NULL)
        {
            //表示没有在stdin里面获取命令时出错
            continue;
        }
        //此时要把cmd_line最后面的回车去掉
        cmd_line[strlen(cmd_line)-1] = '\0';
        //printf("echo: %s\n",cmd_line);
        
        //3. 拆分命令
        //"ls -a -l" ---> "ls" "-a" "-l"
        //strtok
        //第一次调用 -- 要传入原始字符串
        //如果还要继续解析原字符串 -- 传入NULL
        g_argv[0] = strtok(cmd_line,SEP);
        int idx = 1;
        //可以把ls的颜色加一下
        if(strcmp(g_argv[0],"ls") == 0)
        {
            g_argv[idx++] = "--color=auto";
        }
        while(g_argv[idx++] = strtok(NULL,SEP)); //这种写法 -- 如果返回NULL，子串提取完成
        //测试一下看看提取的对不对
        //for(idx = 0;g_argv[idx];idx++)
        //{
        //    printf("g_argv[%d]: %s\n",idx,g_argv[idx]);
        //}
        
        //4. TODO
        if(strcmp(g_argv[0],"cd")==0)
        {
            //让父进程执行 -- 不要创建子进程
            //内置命令（内建命令） -- 本质就是shell中的一个函数调用
            //我们用一个系统调用 -- chdir
            if(g_argv[1]!=NULL)chdir(g_argv[1]);
            continue;
        }
        //导入环境变量
        // if(strcmp(g_argv[0],"export") == 0 && g_argv[1]!=NULL)
        // {
        //     int ret = putenv(g_argv[1]);
        //     if(ret == 0) printf("%s export success\n",g_argv[1]);
        //     continue;
        // }

        //5. fork()
        pid_t id = fork();
        if(id == 0)
        {
            //子进程
            //选择一个程序替换的接口
            //我们肯定要选带v和带p的 -- 因为指针数组要准备好了，而且命令都在PATH中
            //为了方便 -- 带execvp是最好的
            printf("下面功能是让子进程执行的\n");
            execvpe(g_argv[0],g_argv,environ);
            exit(1);
        }
        //父进程 -- 这里我们不用else了 -- 子进程执行完直接退出, 后面的肯定是父进程了
        int status = 0;
        pid_t ret = waitpid(id,&status,0);
        if(ret > 0)
        {
            printf("exit code: %d\n",WEXITSTATUS(status));
        }

    }
    return 0;
}
