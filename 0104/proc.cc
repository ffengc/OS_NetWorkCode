

#include <iostream>
#include <unistd.h>
using namespace std;


int main()
{
    while(1)
    {
        sleep(1);
        cout << getpid() << endl;
    }
    return 0;
}