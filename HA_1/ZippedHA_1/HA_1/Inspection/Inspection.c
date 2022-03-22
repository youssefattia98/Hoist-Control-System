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
int  watchdogPID,motorzPID,motorxPID; 

FILE *log_file_poin;

void logfile()
{
  log_file_poin = fopen("./../Textfiles/log","a");
  if (log_file_poin){
    //printf("log file successfully opened\n");
  }
  else{
    printf("Failed to open the file\n");
  }
}

void readfileX()
{
    /*
        This function reads the file that stores the Motor X process ids
    */
    FILE *fp;
    char pids[100];
  
    // Open file in reding mode
    fp = fopen("./../Textfiles/pidMotorX","r");
    logfile();
    fprintf(log_file_poin, "Inspection: Read the motor X PID\n");
    fclose(log_file_poin);

    // If file opened successfully, then write the string to file
    if ( fp )
   {
       while(fscanf(fp, "%s", senstr)!=EOF)
        {
           sscanf(senstr, "%d",&motorxPID);
          
        }  
   }
   else
    {
         printf("Failed to open the file\n");
         logfile();
         fprintf(log_file_poin, "Inspection: Faild to open the file X\n");
         fclose(log_file_poin);
    }
    //Close the file
    //fclose(fp);
}


void readfileZ(){
    /*
        This function reads the file that stores the Motor X process ids
    */

    FILE *fp;
    char pids[100];
  
    // Open file in reading mode
    fp = fopen("./../Textfiles/pidMotorZ","r");
    logfile();
    fprintf(log_file_poin, "Inspection: Read the motor Z PID\n");
    fclose(log_file_poin);

    // If file opened successfully, then write the string to file
    if ( fp )
   {
       while(fscanf(fp, "%s", senstr)!=EOF)
        {
           sscanf(senstr, "%d",&motorzPID);
        }  
   }
   else
    {
        printf("Failed to open the file\n");
        logfile();
        fprintf(log_file_poin, "Inspection: Faild to open the file Z\n");
        fclose(log_file_poin);
    }
    //Close the file 
    //fclose(fp);
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

        //Process that reads from the motors
        son=fork();
        if (son==0)
        {
        fd2 = open(inspectionx,O_RDONLY);
        fd3 = open(inspectionz,O_RDONLY); 
        read(fd2, str2, 80);
        logfile();
        fprintf(log_file_poin, "Inspection: Reading postion from motor x is ");
        fprintf(log_file_poin, "%s\n", str2);
        fclose(log_file_poin);

        read(fd3, str3, 80);
        logfile();
        fprintf(log_file_poin, "Inspection: Reading postion from motor z is ");
        fprintf(log_file_poin,"%s\n", str3);
        fclose(log_file_poin);
        printf("motor x:");
        puts(str2);
        fflush(stdout);
        printf("motor z:");
        puts(str3);
        fflush(stdout);
        watchdogPID = atoi(argv[1]);                        // convert from string to char
        kill(watchdogPID, SIGUSR1);                         //send a signal to the watchdog
        }
        else
        {
            //Process that reads form the user input
            printf("Press s-->STOP\n");
            printf("Press r-->Reset\n");
            char input[80]="";
            char stop[]="s\n";
            char reset[]="r\n";
            fgets(input, 80 , stdin);
            if (!strcmp(input,stop))
            {
                printf("process should stop\n");
                kill(motorxPID, SIGSTOP);                //send a signal to the motorx
         	    kill(motorzPID, SIGSTOP);               //send a signal to the motorz
                logfile();
                fprintf(log_file_poin, "Inspection: User pressed STOP to motorx and motor z \n");
                fclose(log_file_poin);
                input[0]='\0';
                

            }
            else if (!strcmp(input,reset))
            {
                printf("process should reset\n");
                kill(motorxPID, SIGUSR2);                //send a signal to the motorx
         	    kill(motorzPID, SIGUSR2);               //send a signal to the motorz
                logfile();
                fprintf(log_file_poin, "Inspection: The user pressed RESET to motorx and motor z \n");
                fclose(log_file_poin);
            }
            else
            {
                printf("Wrong input\n");
                logfile();
                fprintf(log_file_poin, "Inspection: The user pressed wrong input \n");
                fclose(log_file_poin);
            }
        }
    } 
    return 0; 
}