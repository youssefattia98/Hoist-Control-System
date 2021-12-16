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
int watchdogPID;
char watchdogPIDstr[5];
FILE *log_file_poin;


typedef enum 
{
  false,
  true
} bool;
int const SLEEP_TIMER = 60;
bool commandsig = true;

void sig_handler (int signo) {
  /*
    This function handles every signal that arrives to this process
  */
  if (signo == SIGUSR1) 
  { 
    //if signal is recived change this boolean to false
    commandsig = false;
  }
  if (signo== SIGUSR2)
  {
    // if 'q' is an input all the pocesses wil terminate
    kill(inspection,SIGTERM);
    exit(EXIT_SUCCESS);
  }
}

void logfile(){
  log_file_poin = fopen("./../Textfiles/log","a");
  if (log_file_poin){
    //printf("log file successfully opened\n");
  }
  else{
    printf("Failed to open the file\n");
  }
}

int main () 
{
  // registering signal handler
  struct sigaction sig;
  memset(&sig, 0, sizeof(sig));
  sig.sa_handler = &sig_handler;
  sigaction(SIGUSR1, &sig, NULL);
  sigaction(SIGUSR2, &sig, NULL);

  command = fork();
  // exec the command process
  if (command == -1) {
    printf("Command process error in creating\n");
    logfile();
    fprintf(log_file_poin, "Master: Command process error in creating\n"); 
    fclose(log_file_poin);
    }
  if (command==0) 
  {
    watchdogPID = getppid();
    sprintf(watchdogPIDstr, "%d", watchdogPID);                                      // integer to string
    char * arg_list_1[] = { "/usr/bin/konsole",  "-e", "./../Command/command",(char*)watchdogPIDstr, (char*)NULL };
    if(execvp ("/usr/bin/konsole", arg_list_1)==-1){
      printf("Command procces failed to exec\n");
      logfile();
      fprintf(log_file_poin, "Master: Command procces failed to exec\n"); 
      fclose(log_file_poin);

    }
  }

  else
  {
    // exec the inspection process
    inspection = fork();
    if (inspection == -1) {
      printf("Inspection process error in creating\n");
      logfile();
      fprintf(log_file_poin, "Master: Inspection process error in creating\n"); 
      fclose(log_file_poin);
    }
    if(inspection==0)
    {
      watchdogPID = getppid();
      sprintf(watchdogPIDstr, "%d", watchdogPID);                                     // integer to string
      char * arg_list_2[] = { "/usr/bin/konsole",  "-e", "./../Inspection/inspection",(char*)watchdogPIDstr,(char*)NULL };
      if(execvp ("/usr/bin/konsole", arg_list_2)==-1){
        printf("Command procces failed to exec\n");
        logfile();
        fprintf(log_file_poin, "Master: Command procces failed to exec\n"); 
        fclose(log_file_poin);
    }
      
    }
    else
    {
      motorx = fork();
      if (motorx == -1){
        printf("motorx process error in creating\n");
        logfile();
        fprintf(log_file_poin, "Master: motorx process error in creating\n"); 
        fclose(log_file_poin);
      }
      if (motorx==0)
      {
        watchdogPID = getppid();
        sprintf(watchdogPIDstr, "%d", watchdogPID);
        /* exec the motorx process */
        char *args[]={"./../MotorX/motorx",(char*)watchdogPIDstr,NULL};
        if(execvp(args[0],args)==-1){
          printf("Motorx procces failed to exec\n");
          logfile();
          fprintf(log_file_poin, "Master: Motorx procces failed to exec\n"); 
          fclose(log_file_poin);
        }
      }
      else
      {
        motorz = fork();
        if (motorz == -1){
          printf("Motorz process error in creating\n");
          logfile();
          fprintf(log_file_poin, "Master: Motorz process error in creating\n"); 
          fclose(log_file_poin);
        }
        if (motorz==0)
        {
          watchdogPID = getppid();
          sprintf(watchdogPIDstr, "%d", watchdogPID);                                  // convert integer to string
          /* exec the motorz process  */
          char *args[]={"./../MotorZ/motorz",(char*)watchdogPIDstr,NULL};
          if(execvp(args[0],args)==-1){
            printf("Motorx process error in creating\n");
            logfile();
            fprintf(log_file_poin, "Master: Motorx process error in creating\n"); 
            fclose(log_file_poin);

          }
        }
        else
        {
          printf("All process succeeded in creating\n");
          printf("Command PID:%d\n",command);
          printf("MotorZ PID:%d\n",motorz);
          printf("MotorX PID:%d\n",motorx);
          printf("Inspection PID:%d\n",inspection);
          watchdogPID = getpid();
          printf("Master PID:%d\n",watchdogPID);
          logfile();
          fprintf(log_file_poin, "Master: All process succeeded in creating\n");
          fprintf(log_file_poin, "Master:Command PID:%d\n",command);
          fprintf(log_file_poin, "Master: MotorZ PID:%d\n",motorz);
          fprintf(log_file_poin, "Master:MotorX PID:%d\n",motorx);
          fprintf(log_file_poin, "Master:Inspection PID:%d\n",inspection);
          fclose(log_file_poin);
          while (1) 
          {
            sleep(SLEEP_TIMER);
            if (commandsig) 
            {
              printf("Watchdog: Terminating processes...\n");
              logfile();
              fprintf(log_file_poin, "Master: Watchdog: Terminating processes...\n");
              fclose(log_file_poin);
              kill(motorx, SIGUSR2);                                                 //send a signal to the motorx to reset them
              kill(motorz, SIGUSR2);                                                 //send a signal to the motorz to reset them
              return 0;
            }
            else 
            {
              // The processes sent the signal correctly and the watchdog timer is resetting
              commandsig = true;
              printf("Watchdog: Timer reset\n");
              logfile();
              fprintf(log_file_poin, "Master: Watchdog: Timer reset\n");
              fclose(log_file_poin);
            }
          }
        }
      }
    }
  }
  return 0;
}