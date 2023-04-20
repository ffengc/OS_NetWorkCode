

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


//输出重定向
#if false
int main()
{
    close(1);
    int fd = open("log.txt",O_WRONLY|O_CREAT|O_TRUNC,0666);
    if(fd < 0)
    {
        perror("open");
        return 1;
    }
    printf("fd: %d\n",fd);
    fflush(stdout);
    close(fd);
    return 0;
}
#endif

//输入重定向
#if false
int main()
{
    close(0);//把stdin删掉
    int fd = open("log.txt",O_RDONLY);
    if(fd<0)
    {
        perror("open");
        return 1;
    }
    printf("fd: %d\n",fd);//此时就是0了
    char buffer[64];
    fgets(buffer,sizeof buffer,stdin);
    printf("%s\n",buffer);

    close(fd);
    return 0;
}
#endif


//追加重定向
int main()
{
    close(1);
    // int fd = open("log.txt",O_WRONLY|O_TRUNC|O_CREAT);
    int fd = open("log.txt",O_WRONLY|O_APPEND|O_CREAT);
    if(fd<0)
    {
        perror("open");
        return 1;
    }
    fprintf(stdout,"you can see me, success\n"); //这里本来是要往显示器打印的
    //现在是要往 log.txt 里面去打印
    return 0;
}