#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h>





char X_input[80], Z_input[80], X_output[80], Z_output[80];
void setting(){
    char inc[] = "Increase";
    char dec[] = "Decrease";
    char still[] = "Still";
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
int main(){
    int X;
    char * commandx = "/tmp/commandx"; 
    mkfifo(commandx, 0666);
    //char * commandz = "/tmp/commandz"; 
    //mkfifo(commandz, 0666);
    

    while (1){
        X = open(commandx, O_WRONLY); 
        //int Z = open(commandz, O_WRONLY);
        printf("Please enter command for motor X\n");
        fflush(stdout);
        fgets(X_input, 80 , stdin);
        // printf("Please enter command for motor Z\n");
        // fflush(stdout);
        // fgets(Z_input, 80 , stdin);
        setting();
        write(X, X_output, strlen(X_output)+1); 
        //write(Z, Z_output, strlen(Z_output)+1);
        close(X);
        //close(Z);
    }

    return 0;
}