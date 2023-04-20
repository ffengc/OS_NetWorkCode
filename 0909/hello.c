#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>

int g_unval;//未初始化全局变量
int g_val =  100;//初始化全局变量

int main(int argc, char* argv[], char* env[])
{
    printf("code addr: %p\n",main);
    printf("init global addr: %p\n",&g_val);
    printf("uninit global addr: %p\n",&g_unval);

    char *heap_mem  = (char*)malloc(10);//堆上的空间
    printf("heap addr: %p\n",heap_mem);

    printf("stack addr: %p\n",&heap_mem);//栈区的地址
    
    for(int i = 0;i < argc; i++)
    {
        printf("argv[%d]: %p\n",i,argv[i]);
    }
    for(int i = 0; env[i] ; i++)
    {
        printf("env[%d]: %p\n",i,env[i]);
    }
    return 0;
}
