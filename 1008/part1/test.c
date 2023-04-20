

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 

int main()
{
    int max_cnt = 0;
    while(1)
    {
        pid_t id = fork();
        if(id < 0)
        {
            printf("fork error: %d\n",max_cnt);
            break;
        }
        else if(id == 0)
        {
            while(1)
            {
                sleep(1);//不要让子进程退出
            }
        }
        else 
        {
            //father
        }
        max_cnt++;
    }
    return 0;
}
