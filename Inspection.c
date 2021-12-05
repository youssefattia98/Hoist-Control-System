#include <stdio.h>
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <signal.h>
#include <ctype.h>

char senstr[100];
int fd,fd2, watchdogPID,motorzPID,motorxPID,commandPID,inspectionPID; 

void readfileX(){
  FILE *fp;
  char pids[100];
  
  // Open file in write mode
   fp = fopen("/home/youssefattia/Desktop/ARPASSNEW/pidMotorX","r");

   // If file opened successfully, then write the string to file
   if ( fp ){
       while(fscanf(fp, "%s", senstr)!=EOF){
           sscanf(senstr, "%d",&motorxPID);
           printf("%d\n",motorxPID);
           }  
   }
   else
      {
         printf("Failed to open the file\n");
        }
//Close the file
   fclose(fp);
}

// reading form the file that stores the Motor Z processes ids
void readfileZ(){
    FILE *fp;
  char pids[100];
  
  // Open file in write mode
   fp = fopen("/home/youssefattia/Desktop/ARPASSNEW/pidMotorZ","r");

   // If file opened successfully, then write the string to file
   if ( fp ){
       while(fscanf(fp, "%s", senstr)!=EOF){
           sscanf(senstr, "%d",&motorzPID);
           //printf("%d\n",motorzPID);
           }  
   }
   else
      {
         printf("Failed to open the file\n");
        }
//Close the file 
}
int main(int argc, char * argv[]) 
{
    
    int fd2, fd3;
    int son=0; 

    char * inspectionx = "/tmp/inspectionx"; 
    char * inspectionz = "/tmp/inspectionz"; 

    char str2[80]="0";
    char str3[80]="0";
    char pidstr[80]="0";
    while (1) 
    {
        readfileX();
        readfileZ();
        sleep(1);
        son=fork();
        if (son==0){
        fd2 = open(inspectionx,O_RDONLY);
        fd3 = open(inspectionz,O_RDONLY); 
        read(fd2, str2, 80);
        read(fd3, str3, 80);
        printf("motor x:");
        puts(str2);
        fflush(stdout);
        printf("motor z:");
        puts(str3);
        fflush(stdout);
        watchdogPID = atoi(argv[1]);
        kill(watchdogPID, SIGUSR1); //send a signal to the watchdog
        }
        else{
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
                printf("i will stop:%d\n",motorxPID);
                kill(motorxPID, SIGSTOP); //send a signal to the motorx
         	    kill(motorzPID, SIGSTOP); //send a signal to the motorz
                input[0]='\0';
                

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
