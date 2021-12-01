#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <string.h>


int main (int argc, char * argv[]) {
  while (1)
  {
    int x=0,pid;
    pid = atoi(argv[1]);


    kill(pid, SIGUSR1); //send a signal to the watchdog
  }
  

  return 0;
}