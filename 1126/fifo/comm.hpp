#ifndef _COMM_H_
#define _COMM_H_

#include <iostream>
#include <string>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Log.hpp"

using namespace std;

#define MODE 0666
#define SIZE 128
string ipcPath = "./fifo.ipc"; //这个是我们管道文件的路径

#endif