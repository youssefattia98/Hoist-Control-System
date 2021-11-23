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





int main(){
    int X;
    char * commandx = "/tmp/commandx"; 
    mkfifo(commandx, 0666);
    char X_input[80], Z_input[80], X_output[80], Z_output[80];
    
    while (1){
    X = open(commandx, O_WRONLY);
    read(X, X_input, 80);
    puts(X_input);
    fflush(stdout);
    close(X);
    }
    
    
    return 0;
}