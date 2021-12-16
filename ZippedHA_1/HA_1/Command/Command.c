#include <stdio.h>
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <signal.h>
#include <ctype.h>

char X_input[80], Z_input[80],  X_output[80], Z_output[80];
char senstr[100];
int fd,fd2, watchdogPID,motorzPID,motorxPID; 
FILE *log_file_poin;

void logfile(){
  log_file_poin = fopen("./../Textfiles/log","a");
  if (log_file_poin){
    //printf("log file successfully opened\n");
  }
  else{
    printf("Failed to open the file\n");
  }
}


void readfileX(){
    /*
        This function reads the file that stores the Motor X process ids
    */

  FILE *fp;
  char pids[100];
  
   // Open file in reading mode
    fp = fopen("./../Textfiles/pidMotorX","r");
    logfile();
    fprintf(log_file_poin, "Command: Read the motor X PID\n");
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
         fprintf(log_file_poin, "Command: Faild to open the file X\n");
         fclose(log_file_poin);
        }
    //Close the file
    fclose(fp);
}


void readfileZ(){
    /*
        This function reads the file that stores the Motor Z process ids
    */

    FILE *fp;
    char pids[100];
  
    // Open file in reading mode
    fp = fopen("./../Textfiles/pidMotorZ","r");
    logfile();
    fprintf(log_file_poin, "Command: Read the motor Z PID\n");
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
         fprintf(log_file_poin, "Command Faild to open the file Z\n");
         fclose(log_file_poin);
        }
    //Close the file 
    fclose(fp);
}


void setting(){
    /*
        This function handels the instruction given by the user and creates
        the string that will be sent in the FIFO.
    */
    char inc[] = "Inc";
    char dec[] = "Dec";
    char still[] = "Sti";
    switch (X_input[0])
    {
    case 'j':
        strcpy(X_output,inc);
        logfile();
        fprintf(log_file_poin, "Command: The user pressed INCREASE in motor x\n");
        fclose(log_file_poin);
        break;
    case 'l':
        strcpy(X_output,dec);
        logfile();
        fprintf(log_file_poin, "Command: The user pressed DECREASE in motor x\n");
        fclose(log_file_poin);
        break;
    default:
        strcpy(X_output,still);
        logfile();
        fprintf(log_file_poin, "Command: The user pressed STILL in motor x\n");
        fclose(log_file_poin);
        break;
    }

    switch (Z_input[0])
    {
    case 'i':
        strcpy(Z_output,inc);
        logfile();
        fprintf(log_file_poin, "Command: The user pressed INCREASE in motor z\n");
        fclose(log_file_poin);
        break;
    case 'k':
        strcpy(Z_output,dec);
        logfile();
        fprintf(log_file_poin, "Command: The user pressed DECREASE in motor z\n");
        fclose(log_file_poin);
        break;
    default:
        strcpy(Z_output,still);
        logfile();
        fprintf(log_file_poin, "Command: The user pressed STILL in motor z\n");
        fclose(log_file_poin);
        break;
    }
}

int main(int argc, char * argv[]) 
{
    printf("Command succeded in execting\n");
    logfile();
    fprintf(log_file_poin, "Command: Command succeded in execting\n"); 
    fclose(log_file_poin);
    // FIFO pipes opening
    char * commandX = "/tmp/commandX"; 
    mkfifo(commandX, 0666);
    
    char * commandZ = "/tmp/commandZ"; 
    mkfifo(commandZ, 0666);

    while (1) 
    {
        readfileX();
        readfileZ();
        fd = open(commandX, O_WRONLY); 
        fd2 = open(commandZ,O_WRONLY);
        logfile();
        fprintf(log_file_poin, "Command: Opened 2 fifos for commandX and commandZ\n"); 
        fclose(log_file_poin);

        watchdogPID = atoi(argv[1]);

        printf("Please enter motor x command, q will terminate all process.\n");
        printf("j: Increase\n");
        printf("l: Decrease\n");
        printf("Any bottom: still\n");
        fflush(stdout);
        //Print the user input for motorX
        fgets(X_input, 80 , stdin); 
        fflush(stdout);

        printf("Please enter motor z command, q will terminate all process.\n");
        printf("i: Increase\n");
        printf("k: Decrease\n");
        printf("Any bottom: Still\n");
        fflush(stdout);
        // Print the user inoput for motorZ
        fgets(Z_input, 80 , stdin);
        fflush(stdout);
        logfile();
        fprintf(log_file_poin, "Command: User enterd commands for motors\n"); 
        fclose(log_file_poin);

        // q input terminates all the processes
        if (Z_input[0] == 'q'||X_input[0] == 'q')
        {
            X_output[0]= 'q';
            Z_output[0]= 'q';
            write(fd, X_output, strlen(Z_input)+1);
            write(fd2, Z_output, strlen(Z_input)+1);
            logfile();
            fprintf(log_file_poin, "Command: The user pressed q terminate all processes\n");
            fclose(log_file_poin);
            unlink("/tmp/commandX");
            unlink("/tmp/commandZ");
            unlink("/tmp/inspectionx");
            unlink("/tmp/inspectionz");
            kill(watchdogPID,SIGUSR2);
            exit(EXIT_SUCCESS) ;
        }

        setting();
        /* 
            Signals that make the motors move again after the STOP singal has
            been sent by the Inspection console.
        */
        logfile();
        fprintf(log_file_poin, "Command: sending continue signal to motors\n"); 
        fclose(log_file_poin);
        kill(motorxPID,SIGCONT);
        kill(motorzPID,SIGCONT);
        
        // Writing in the FIFO
        
        write(fd, X_output, strlen(Z_input)+1); 
        write(fd2, Z_output, strlen(Z_input)+1); 
        logfile();
        fprintf(log_file_poin, "Command: Sent strings to fifos\n"); 
        fclose(log_file_poin);
        close(fd);
        close(fd2);
        //send a signal to the watchdog
        kill(watchdogPID, SIGUSR1); 
    }
    return 0; 
} 