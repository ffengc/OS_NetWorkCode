



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


// int main()
// {
//     fprintf(stdout,"hello stdout\n");
//     const char* s = "hello 1\n";

//     write(1,s,strlen(s));
//     return 0;
// }


int main()
{
    printf("stdin: %d\n",stdin->_fileno);
    printf("stdout: %d\n",stdout->_fileno);
    printf("stderr: %d\n",stderr->_fileno);
    return 0;
}