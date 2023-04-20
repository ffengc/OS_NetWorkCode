

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


//用dup2重定向

//功能
//myfile helloworld   --- 把helloworld写到文件里
int main(int argc, char* argv[])
{
    umask(0);
    if(argc!=2)return 2;
    int fd = open("log.txt",O_WRONLY|O_CREAT|O_TRUNC,0666);
    if(fd < 0)
    {
        perror("open");
        return 1;
    }
    //重定向！
    dup2(fd,1);
    printf("%s\n",argv[1]); //stdout -> 1
    close(fd);
    return 0;
}