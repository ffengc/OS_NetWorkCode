

#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string>
using namespace std;

static void Usage(string proc)
{
    cout << "Usage:\r\n\t" << proc << "signumber processid" << endl;
}
int main(int argc, char *argv[])
{
    cout << "我开始运行了" << endl;
    sleep(1);
    raise(8);
    return 0;
}