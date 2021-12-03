#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <ctype.h>

//Global varibales
int Zpos, Zesti_pos = 0;
float err=0;
char rec[80]="";
char sen[80]= "from motor z"; //output sting

float generror(){

    /*
    This function returns a random number between 0 and 1 as float
    */
    float error;
    srand ( time(NULL) );
    return error = (double)rand() / (double)RAND_MAX ;
}


int main(int argc, char * argv[]) 
{ 
    int fd2,fd3,watchdogPID; 

    char * commandZ = "/tmp/commandZ"; 
    char * inspectionz = "/tmp/inspectionz";
    mkfifo(inspectionz, 0666); 
    


    while (1) 
    {
        fd2 = open(commandZ,O_RDONLY);
        fd_set rfds;
        struct timeval tv;
        int retval;
        FD_ZERO(&rfds);
        FD_SET(fd2, &rfds);
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        retval = select(fd2+1, &rfds, NULL, NULL, &tv);

        if (retval == -1){
            perror("error!");
        }
        else if (retval){
            /*
            data is now available
            */
           read(fd2, rec, 80);
           //should call function here to edit the sent stirng
           fd3 = open(inspectionz,O_WRONLY);
           write(fd3, sen, strlen(sen)+1);
        }
        else{
            /*
            no data update
            */
           fd3 = open(inspectionz,O_WRONLY);
           write(fd3, sen, strlen(sen)+1);
        }
        watchdogPID = atoi(argv[1]);
        kill(watchdogPID, SIGUSR1); //send a signal to the watchdog
    } 
    return 0; 
} 
