


#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string>
using namespace std;


//怎么调用 ./mykill 2 pid
static void Usage(string proc)
{
    cout << "Usage:\r\n\t" << proc << "signumber processid" << endl;
}
int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        Usage(argv[0]);
        exit(1);
    }

    int signumber = atoi(argv[1]);
    int procid = atoi(argv[2]);
    kill(procid,signumber);
    return 0;
}