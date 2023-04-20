

#include "mymath.h"

int addToTarget(int from,int to)
{
    int sum = 0;
    int i = 0;
    for(i = from; i<=to; i++)
    {
        sum += i;
    }
    return sum;
}