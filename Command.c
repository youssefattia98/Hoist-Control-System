#include <stdio.h>
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>

char X_input[80], Z_input[80],  X_output[80], Z_output[80];


void setting(){
    char inc[] = "Inc";
    char dec[] = "Dec";
    char still[] = "Stil";
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


int main() 
{ 
    int fd,fd2; 
    char * commandX = "/tmp/commandX"; 
    mkfifo(commandX, 0666);
    
    char * commandZ = "/tmp/commandZ"; 
    mkfifo(commandZ, 0666); 

    int id=fork();

    if (id==0){
		//printf("Hello from the child process\n");
        /*
        this needs to exec motorx and motorz
        check: 
        week 7 and https://www.youtube.com/watch?v=OVFEWSP7n8c&t=466s&ab_channel=CodeVault
        */
	}

    else{
		printf("Hello from the father process\n");
        fflush(stdout);

        
        while (1) { 
        fd = open(commandX, O_WRONLY); //opens the file
        fd2 = open(commandZ,O_WRONLY); 
        printf("Please enter motor x command\n");
	    fflush(stdout);
        fgets(X_input, 80 , stdin); //print what i have just inputted
        fflush(stdout);

        printf("Please enter motor z command\n");
	    fflush(stdout);
        fgets(Z_input, 80 , stdin);
        fflush(stdout);

        setting();
        write(fd, X_output, strlen(Z_input)+1); //writes in file
        write(fd2, Z_output, strlen(Z_input)+1); 
        close(fd); //close the file
        close(fd2); 
        }
	} 
    return 0; 
} 

