


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int main()
{
    umask(0);
    // int fd = open("log.txt",O_WRONLY|O_CREAT|O_TRUNC,0666); //rw-rw-rw-
    int fd = open("log.txt",O_RDONLY);
    // int fd = open("log.txt",O_WRONLY|O_CREAT|O_APPEND,0666);
    if(fd < 0)
    {
        perror("open");
        return 1;
    }
    //open success
    printf("open success, fd: %d\n",fd);
    // //写东西
    // const char* s = "aa";
    // write(fd,s,strlen(s));//要不要+1 不用！

    //读东西
    char buffer[64] ;
    memset(buffer,'\0',sizeof(buffer));//保证字符串的结尾是\0 read是不会帮我们加的
    read(fd,buffer,sizeof(buffer));

    printf("%s",buffer);


    //关闭文件
    close(fd);
    return 0;
}