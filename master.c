#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <signal.h>
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 

int command,  inspection, motorx , motorz;
char senstr[]="";

void sendingstring(){
  char commandPIDstr[10]="";
  sprintf(commandPIDstr, "%d", command);
  char inspectionPIDstr[10]="";
  sprintf(inspectionPIDstr, "%d", inspection);
  char motorxPIDstr[10]="";
  sprintf(motorxPIDstr, "%d", motorx);
  char motorzPIDstr[10]="";
  sprintf(motorzPIDstr, "%d", motorz);
  commandPIDstr[strlen(commandPIDstr)]=',';
  inspectionPIDstr[strlen(inspectionPIDstr)]=',';
  motorxPIDstr[strlen(motorxPIDstr)]=',';
  strcat(senstr, commandPIDstr);
  strcat(senstr, inspectionPIDstr);
  strcat(senstr, motorxPIDstr);
  strcat(senstr, motorzPIDstr);
  }
  
typedef enum {
  false,
  true
} bool;
int const SLEEP_TIMER = 60;
bool commandsig = true;


void sig_handler (int signo) {
  if (signo == SIGUSR1) { //if signal is recived change this boolen to false
    commandsig = false;
  }
}

int watchdogPID;
char watchdogPIDstr[5];
char motorzPIDstr[5];
char motorxPIDstr[5];


int main () {
  // registering signal handler
  struct sigaction sig;
  memset(&sig, 0, sizeof(sig));
  sig.sa_handler = &sig_handler;
  sigaction(SIGUSR1, &sig, NULL);

  command = fork();
  if (command==0){
    /*
    Should exec the command process
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
      watchdogPID = getppid();
      sprintf(watchdogPIDstr, "%d", watchdogPID); // integer to string
      char * arg_list_2[] = { "/usr/bin/konsole",  "-e", "./inspection",(char*)watchdogPIDstr,(char*)NULL };
      execvp ("/usr/bin/konsole", arg_list_2);
      
     }
     else{
       motorx = fork();
       printf("motor x pid is:%d",motorx);
       if (motorx==0){
         watchdogPID = getppid();
         sprintf(watchdogPIDstr, "%d", watchdogPID);
         /* should exec the motorx process */
         char *args[]={ "/usr/bin/konsole",  "-e","./motorx",(char*)watchdogPIDstr,NULL};
         execvp(args[0],args);
       }
       else{
         motorz = fork();
         if (motorz==0)
         {
           watchdogPID = getppid();
           sprintf(watchdogPIDstr, "%d", watchdogPID); // integer to string
           /* should exec the motorz process  */
           char *args[]={"./motorz",(char*)watchdogPIDstr,NULL};
           execvp(args[0],args);
         }



         else{
           sendingstring();
          
           while (1) {
             sleep(SLEEP_TIMER);
             if (commandsig) {
               printf("Watchdog: Terminating processes...\n");
               kill(command, SIGKILL); //should be changed
               return 0;
               }
            else {
              // reset "killing"
              commandsig = true;
              printf("Watchdog: Timer reset\n");
              }
            }
         }
       }
     }
  }
  return 0;
}