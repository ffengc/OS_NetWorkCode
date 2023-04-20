

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


#define NUM 16

//const char *myfile = "/home/yufc/bit/0921/mycmd";//这里要把我们要替换的文件路径写好

const char *myfile = "./mycmd";//如果使用相对路径呢？


int main()
{
    pid_t id = fork();
    
    char *const _env[NUM] = {"MY_105_VAL=888777666",
                        NULL};


    if(id == 0)
    {
        //子进程
        printf("子进程开始运行, pid: %d\n",getpid());
        
        execle(myfile,"mycmd","-a",NULL,_env);
        
        //执行其他语言的程序
        
        //execlp("python","python","test.py",NULL); 
        //execlp("./test.py","test.py",NULL);
        exit(1);
    }
    else
    {
        //父进程
        printf("父进程开始运行, pid: %d\n",getpid());
        int status = 0;
        pid_t id = waitpid(-1,&status,0);//阻塞等待 -- 子进程运行完毕 -- 然后父进程再执行
        if(id > 0)
        {
            //等待成功
            printf("wait success, exit code: %d\n",WEXITSTATUS(status));
        }
    }
    return 0;
}
