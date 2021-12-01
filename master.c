#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <string.h>

typedef enum {
  false,
  true
} bool;
int const SLEEP_TIMER = 10;
bool isKiller = true;



void sig_handler (int signo) {
  if (signo == SIGUSR1) { //if signal is recived change this boolen to false
    isKiller = false;
  }
}

int watchdogPID;
char watchdogPIDstr[5];
int main () {
  pid_t console,  inspection, motorx , motorz;
  int x;

  // registering signal handler
  struct sigaction sig;
  memset(&sig, 0, sizeof(sig));
  sig.sa_handler = &sig_handler;
  sigaction(SIGUSR1, &sig, NULL);

  console = fork();
  if (console==0){
    /*
    Should exec the console process
    */
    watchdogPID = getppid();
    sprintf(watchdogPIDstr, "%d", watchdogPID); // integer to string
    char * arg_list_1[] = { "/usr/bin/konsole",  "-e", "./command",(char*)watchdogPIDstr, (char*)NULL };
    execvp ("/usr/bin/konsole", arg_list_1);
  }

  else{
     inspection = fork();
     if(inspection==0){
       /*
       Should exec the inspection process
       */
      char * arg_list_2[] = { "/usr/bin/konsole",  "-e", "./inspection",(char*)watchdogPIDstr, (char*)NULL };
      execvp ("/usr/bin/konsole", arg_list_2);
      
     }
     else{
       motorx = fork();
       if (motorx==0){
         /* should exec the motorx process */
         char *args[]={"./motorx",NULL};
         execvp(args[0],args);
       }
       else{
         motorz = fork();
         if (motorz==0)
         {
           /* should exec the motorz process  */
           char *args[]={"./motorz",NULL};
           execvp(args[0],args);
         }



         else{
           //this is the master which has the watchdog timmer
           while (1) {
             fflush(stdout);
             sleep(SLEEP_TIMER);
             if (isKiller) {
               // kill P1
               printf("Watchdog: Terminating processes...\n");
               kill(console, SIGKILL);
               return 0;
               }
            else {
              // reset "killing"
              isKiller = true;
              printf("Watchdog: Timer reset\n");
              }
            }
         }
       }
     }
  }
  return 0;
}