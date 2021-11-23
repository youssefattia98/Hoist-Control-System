#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
  
int main() 
{ 
    int fd2, fd3; 

    char * inspectionx = "/tmp/inspectionx"; 
    mkfifo(inspectionx, 0666); 

    char * inspectionz = "/tmp/inspectionz"; 
    mkfifo(inspectionz, 0666); 

    char str2[80]="";
    char str3[80]="";

    while (1) 
    {
        fd2 = open(inspectionx,O_RDONLY); 
        read(fd2, str2, 80); 
        puts(str2);
        fflush(stdout);
        close(fd2);

        fd3 = open(inspectionz,O_RDONLY); 
        read(fd3, str3, 80); 
        puts(str3);
        fflush(stdout);
        close(fd3);

    } 
    return 0; 
} 
