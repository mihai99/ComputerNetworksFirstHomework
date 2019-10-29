#pragma once
#define CONFIG_FILE "config"
#define FIFO_PATH "myStatFifo"
#define COMMAND_LOGN 1
#define COMMAND_MYFIND 2
#define COMMAND_MYSTAT 3
#define COMMAND_QUIT 4
#define COMMAND_LOGGOUT 6
#define INVALID_COMMAND 5
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include "utils.h"