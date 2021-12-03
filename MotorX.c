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
int Xpos, Xesti_pos = 0;
float err=0;
char rec[80]="";
char sen[80]="from motor x"; //out sting 

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
    int fd1,fd2,watchdogPID;

     
    char * commandX = "/tmp/commandX"; 
    char * inspectionx = "/tmp/inspectionx"; 
    mkfifo(inspectionx, 0666); 




    while (1) 
    {
        fd1 = open(commandX,O_RDONLY);
        fd_set rfds;
        struct timeval tv;
        int retval;
        FD_ZERO(&rfds);
        FD_SET(fd1, &rfds);
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        retval = select(fd1+1, &rfds, NULL, NULL, &tv);

        if (retval == -1){
            perror("error!");
        }
        else if (retval){
            /*
            data is now available
            */
           read(fd1, rec, 80);
           fd2 = open(inspectionx,O_WRONLY);
           //should call function here to edit the sent stirng
           write(fd2, sen, strlen(sen)+1);
        }
        else{
            /*
            no data update
            */
           fd2 = open(inspectionx,O_WRONLY);
           //should call function here to edit the sent stirng
           write(fd2, sen, strlen(sen)+1);
        }
        watchdogPID = atoi(argv[1]);
        kill(watchdogPID, SIGUSR1); //send a signal to the watchdog
    } 
    return 0; 
}

//if u reach make and min no error is added 
/*
        char inc[] = "Inc";
        char dec[] = "Dec";
        char still[] = "Stil";

        if(!strcmp(rec, inc)){
            //should keep increasing until we reach Xpos=10
            if(Xpos<10){
                Xpos++;
                err=generror();
                Xesti_pos=Xpos+err;
            }

            else{
                err=generror();
                Xesti_pos=Xpos+err;
            }

        }
        if(!strcmp(rec, dec)){
            //should keep decreasing until we reach Xpos=10
            if(Xpos>0){
                Xpos--;
                err=generror();
                Xesti_pos=Xpos+err;
            }

            else{
                err=generror();
                Xesti_pos=Xpos+err;
            }
        }
        if(!strcmp(rec, still)){
            err=generror();
            Xesti_pos=Xpos+err;
        }
*/