

#include "myprint.h"


void Print(const char* str)
{
    printf("%s[%d]\n",str,(int)time(NULL));
}