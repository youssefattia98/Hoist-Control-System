#include <stdio.h>
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <signal.h>
#include <ctype.h>

int main(int argc, char * argv[]) 
{
    int fd2, fd3,watchdogPID,motorzPID,motorxPID,fd4;
    int son=0; 

    char * inspectionx = "/tmp/inspectionx"; 
    char * inspectionz = "/tmp/inspectionz"; 
    char * pids = "/tmp/pids"; 

    char str2[80]="0";
    char str3[80]="0";
    char pidstr[80]="0";
    while (1) 
    {

        son=fork();
        if (son==0){
        fd2 = open(inspectionx,O_RDONLY);
        fd3 = open(inspectionz,O_RDONLY); 
        fd4 = open(pids,O_RDONLY); 
        read(fd4, pidstr, 80);
        puts(pidstr);
        read(fd2, str2, 80);
        read(fd3, str3, 80);
        //printf("motor x speed is in meters per second:");
        //puts(str2);
        fflush(stdout);
        //printf("motor z speed is in meters per second:");
        //puts(str3);
        //printf("m/s");
        fflush(stdout);
        watchdogPID = atoi(argv[1]);
        motorzPID = atoi(argv[2]);
        motorxPID = atoi(argv[3]);
        kill(watchdogPID, SIGUSR1); //send a signal to the watchdog
        }
        else{
            fd4 = open(pids,O_RDONLY); 
            read(fd4, pidstr, 80);
            puts(pidstr);
            printf("Press s-->STOP\n");
            printf("Press r-->Reset\n");
            char input[80]="";
            char stop[]="s\n";
            char reset[]="r\n";
            fgets(input, 80 , stdin);
            printf("you have enetred:");
            puts(input);
            if (!strcmp(input,stop))
            {
                printf("process should stop\n");
                kill(motorxPID, SIGSTOP); //send a signal to the motorx
         	    kill(motorzPID, SIGSTOP); //send a signal to the motorz
            }
            else if (!strcmp(input,reset))
            {
                printf("process should reset\n");
                kill(motorxPID, SIGUSR2); //send a signal to the motorx
         	    kill(motorzPID, SIGUSR2); //send a signal to the motorz
            }
            else{
                printf("Wrong input\n");
            }
        }
    } 
    return 0; 
} 


//it should empty the fifio before recving the signal s
