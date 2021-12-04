#include <stdio.h>
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <signal.h>
#include <ctype.h>
// nothing works because of the file creation

char X_input[80], Z_input[80],  X_output[80], Z_output[80];
char senstr[]="";

void createfile(){
  FILE *fp;
  char pids[100];
  //char senstr[100];

  
   // Open file in write mode
   fp = fopen("/home/youssefattia/Desktop/ARPASSNEW/pids","r");

   // If file opened successfully, then write the string to file
   if ( fp )
   {
	   fgets(senstr,60,fp);
    }
   else
      {
         printf("Failed to open the file\n");
        }
//Close the file
   fclose(fp);
}
void setting(){
    char inc[] = "Inc";
    char dec[] = "Dec";
    char still[] = "Sti";
    switch (X_input[0]){
    case 'j':
        strcpy(X_output,inc);
        break;
    case 'l':
        strcpy(X_output,dec);
        break;
    default:
        strcpy(X_output,still);
        break;
    }
    switch (Z_input[0]){
    case 'i':
        strcpy(Z_output,inc);
        break;
    case 'k':
        strcpy(Z_output,dec);
        break;
    default:
        strcpy(Z_output,still);
        break;
}
}


int main(int argc, char * argv[]) 
{ 
    int fd,fd2, watchdogPID,motorzPID,motorxPID,fd4; 
    char * commandX = "/tmp/commandX"; 
    mkfifo(commandX, 0666);
    
    char * commandZ = "/tmp/commandZ"; 
    mkfifo(commandZ, 0666);

    char * pids = "/tmp/pids"; 
    char pidstr[80]="";
    while (1) { 
        fd = open(commandX, O_WRONLY); //opens the file
        fd2 = open(commandZ,O_WRONLY);
        fd4 = open(pids,O_RDONLY); 

        createfile();
        printf("lets see: %s", senstr);

        //read(fd4, pidstr, 80);
        // puts(pidstr);
        watchdogPID = atoi(argv[1]);
        motorzPID = atoi(argv[2]);
        motorxPID = atoi(argv[3]);
        printf("Please enter motor x command\n");
        printf("j: Increase\n");
        printf("l: Decrease\n");
        printf("Any bottom: Stop\n");
        fflush(stdout);
        fgets(X_input, 80 , stdin); //print what i have just inputted
        fflush(stdout);

        printf("Please enter motor z command\n");
        printf("i: Increase\n");
        printf("k: Decrease\n");
        printf("Any bottom: Stop\n");
        fflush(stdout);
        fgets(Z_input, 80 , stdin);
        fflush(stdout);

        setting();
        kill(motorxPID,SIGCONT);
        kill(motorzPID,SIGCONT);
        write(fd, X_output, strlen(Z_input)+1); //writes in file
        write(fd2, Z_output, strlen(Z_input)+1); 
        close(fd);
        close(fd2);
        kill(watchdogPID, SIGUSR1); //send a signal to the watchdog
    }
    unlink("/tmp/commandX");
    unlink("/tmp/commandZ");
    return 0; 
} 