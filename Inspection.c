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
    int fd2, fd3,watchdogPID; 

    char * inspectionx = "/tmp/inspectionx"; 
    mkfifo(inspectionx, 0666); 

    char * inspectionz = "/tmp/inspectionz"; 
    mkfifo(inspectionz, 0666); 

    char str2[80]="";
    char str3[80]="";

    while (1) 
    {
        fd2 = open(inspectionx,O_RDWR); 
        read(fd2, str2, 80); 
        puts(str2);
        fflush(stdout);
        //close(fd2);

        fd3 = open(inspectionz,O_RDWR); 
        read(fd3, str3, 80); 
        puts(str3);
        fflush(stdout);
        //close(fd3);
        //watchdogPID = atoi(argv[1]);
        //kill(watchdogPID, SIGUSR1); //send a signal to the watchdog
    } 
    return 0; 
} 


//it should empty the fifio before recving the signal s
