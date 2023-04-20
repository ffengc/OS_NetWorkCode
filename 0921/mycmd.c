#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//mycmd -a/-b/-c ... 必须带一个选项，否则不能执行
int main(int argc,char* argv[])
{
    if(argc != 2)
    {
        printf("can not execute! \n");
        exit(1);
    }
    
    //你不是说 -- execle这个函数可以获取环境变量吗？
    //那我们这里打印一下 MY_105_VAL 这个环境变量
    //我们知道 MY_105_VAL 这个环境变量目前不存在
    printf("获取环境变量: MY_105_VAL: %s\n",getenv("MY_105_VAL"));

    if(strcmp(argv[1],"-a") == 0)
    {
        printf("hello a!\n");
    }
    else if(strcmp(argv[1],"b") == 0)
    {
        printf("hello b!\n");
    }
    else 
    {
        printf("default!\n");
    }
    return 0;
}
