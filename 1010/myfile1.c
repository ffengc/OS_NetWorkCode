

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#if 0
int main(int argc, char* argv[])
{
    if(argc!=2)
    {
        printf("argv error!\n");
        return 1;
    }
    FILE* fp = fopen(argv[1],"r");
    if(fp == NULL)
    {
        perror("fopen");
        return 2;
    }
    //进行文件操作
    // const char *s1 = "hello fwrite\n";//不要+1，\0是C语言的规定，文件要遵守吗？文件只需要保存有效数据！！
    // fwrite(s1,strlen(s1),1,fp);
    // const char *s2 = "hello fprintf\n";
    // fprintf(fp,"%s",s2);
    // const char *s3 = "hello fputs\n";
    // fputs(s3,fp);

    //按行读取
    //fgets -> C的接口 -> s(string) -> 会自动在字符串结尾添加\0
    char line[64];
    while(fgets(line,sizeof(line),fp)!=NULL)
    {
        fprintf(stdout,"%s",line);
    }

    fclose(fp);
    return 0;
}
#endif


//用int中不重复的一个bit，就可以标识一种状态
#define ONE 0x1  //0001
#define TWO 0x2  //0010
#define THREE 0x4  //0100

void show(int flags)
{
    if(flags & ONE) printf("hello one!\n");
    if(flags & TWO) printf("hello two!\n");
    if(flags & THREE) printf("hello three!\n");
}
int main()
{
    show(ONE);
    show(TWO);
    show(ONE | TWO);
    show(ONE | TWO | THREE);
    return 0;
}