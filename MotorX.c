#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
  
int main() 
{ 
    int fd1;
    int fd2;
     
    char * commandX = "/tmp/commandX"; 
    mkfifo(commandX, 0666); 

    char * inspectionx = "/tmp/inspectionx"; 
    mkfifo(inspectionx, 0666); 

    char str1[80]="";
    char str2[80]="change later";


    while (1) 
    {
        fd1 = open(commandX,O_RDONLY); 
        read(fd1, str1, 80); //read form command
        puts(str1); //print on terminal what was written
        fflush(stdout);
        close(fd1);

        
        fd2 = open(inspectionx,O_WRONLY);
        write(fd2, str2, strlen(str2)+1);
        close(fd2); 
    } 
    return 0; 
} 
