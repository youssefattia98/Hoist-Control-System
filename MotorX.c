#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <time.h>

float generror(){

    /*
    This function returns a random number between 0 and 1 as float
    */
    float error;
    srand ( time(NULL) );
    return error = (double)rand() / (double)RAND_MAX ;
}



int Xpos, Xesti_pos = 0;
float err=0;
char rec[80]="";
char sen[80]="";

int main() 
{ 
    int fd1;
    int fd2;

     
    char * commandX = "/tmp/commandX"; 
    mkfifo(commandX, 0666); 

    char * inspectionx = "/tmp/inspectionx"; 
    mkfifo(inspectionx, 0666); 




    while (1) 
    {
        fd1 = open(commandX,O_RDWR); 
        //read(fd1, rec, 80); //read form command
        //puts(rec); //print on terminal what was written
        fflush(stdout);
        close(fd1);

         /*
        last command = stop
        loop [
        update/receive command
        if ( command == reset ) <reset procedure>
        if ( command != stop || ~(position == displacement_end) ) [
        position += step
        estimated position = position + error
        <send estimate position>
        ]
        sleep (dt)
        ]
        */

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


        fd2 = open(inspectionx,O_RDWR);
        write(fd2, sen, strlen(sen)+1);
        //close(fd2); 
    } 
    return 0; 
}

//if u reach make and min no error is added 