#include <stdio.h>
#include <unistd.h>

#if 0
int main()
{
  printf("我是父进程\n");
  pid_t id = fork();
  if(id < 0)
  {
    printf("创建子进程失败\n");
    return 1;
  }
  else if(id == 0)
  {
    //子进程
    while(1)
    {
      printf("我是子进程: pid: %d,ppid: %d\n",getpid(),getppid());
      sleep(1);
    }
  }
  else 
  {
    //父进程
    while(1)
    {
      printf("我是父进程: pid: %d,ppid: %d\n",getpid(),getppid());
      sleep(1);
    }
  }
  return 0;
}
#endif

int main()
{
    //我们曾经学习C语言的时候，肯定写过这样一份代码
    const char *str1 = "aaa";
    const char *str2 = "aaa";
    printf("%p\n",str1);
    printf("%p\n",str2);
}