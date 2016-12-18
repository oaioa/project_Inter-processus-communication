#include <stdio.h> //print ...
#include <stdlib.h> //NULL pointer ...
#include <signal.h> //signal ...
#include <unistd.h> //pause ...
#include <time.h> //random ...
#include "semaphore.h" 
#include "shmem.h"//shared memory
#include <sys/msg.h>
#include <sys/types.h>// types : key_t
#include <sys/ipc.h> //IPC permissions
#include <string.h> //strcopy ...

typedef struct vol{
  //int time;
  int number;
  char destination[20];
}VOL;

key_t balK = 16;
key_t memK = 32;
key_t mutexK = 48;
key_t sem_placeK = 64;
