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
  
int main() 
{ 
    int fd2,fd3; 

    char * commandZ = "/tmp/commandZ"; 
    mkfifo(commandZ, 0666);

    char * inspectionz = "/tmp/inspectionz"; 
    mkfifo(inspectionz, 0666); 
    
    char str2[80]="";
    char str3[80]="change later";

    while (1) 
    {
        fd2 = open(commandZ,O_RDWR);
        read(fd2, str2, 80); 
        //puts(str2);
        fflush(stdout);
        //close(fd2);

        fd3 = open(inspectionz,O_RDWR);
        write(fd3, str3, strlen(str3)+1);
        //close(fd3); 
    } 
    return 0; 
} 
