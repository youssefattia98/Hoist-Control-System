#include <stdio.h> 
#include <string.h> 
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <ctype.h>


int Xpos=0;
float Xesti_pos = 0;
float err=0;
char rec[80]="0";
char sen[80]= "0";


float pos=0;
int sign =0;

char senstr[100];
int motorzPID;

// keys 
char inc[] = "Inc";
char dec[] = "Dec";
char still[] = "Sti";
char reset[] = "res";

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

void createfileZ(){
    /*
        This function creates and writes in a file to store the Motor Z process ids
    */
    FILE *fp;
  
    // Open file in write mode
    fp = fopen("./../Textfiles/pidMotorZ","w+");

    // If file opened successfully, then write the string to file
    if ( fp )
    {    
	    motorzPID=getpid();
        sprintf(senstr, "%d", motorzPID);
        logfile();
        fprintf(log_file_poin, "MotorZ: The file created to store PID of motorz which is");
        fprintf(log_file_poin,"%s\n", senstr);
        fclose(log_file_poin);
	    fputs(senstr,fp);
    }
    else
        {
        printf("Failed to open the file\n");
        logfile();
        fprintf(log_file_poin, "MotorZ: Faild to open the file");
        fclose(log_file_poin);
        }
    //Close the file
    fclose(fp);
}

float generror(){
    /*
    This function returns a random number between -0.5 and 0.5 as float
    */
    float error;
    srand ( time(NULL) );
    return error = (double)rand() / (double)RAND_MAX ;
}

int subadd(){
    /*
        This function returns a random number between 0 and 1
    */
    srand ( time(NULL) );
    sign=rand()%2;
    return sign;
}

void handle_sigusr2(int sig){
    /* 
        signal handeler function
    */
    strcpy(rec, reset);
    logfile();
    fprintf(log_file_poin, "MotorZ: The Reset signal is received\n");
    fclose(log_file_poin);
}

float motion(){
    /*
        This is the function that controls the movement of the motors
    */
    if(!strcmp(rec, inc)){
        /*
            Increasing Button will keep increasing until Zpos=10 is reached
        */
        if(Xpos<10){
            Xpos++;
            err=generror();
            sign = subadd();
            err = err/2;
            if(sign==0)
            {
                Xesti_pos=Xpos-err;
            }else
                {
                    Xesti_pos=Xpos+err;
                }
            return Xesti_pos;
        }else
        {
            err=generror();
            Xesti_pos=Xpos;
            return Xesti_pos;
        }

    }
    if(!strcmp(rec, dec))
    {
        /*
            Decreasing button should keep decreasing until we reach Zpos=10
        */
        if(Xpos>0)
        {
            Xpos--;
            err=generror();
            sign = subadd();
            err = err/2;
            if(sign==0)
            {
                Xesti_pos=Xpos-err;
            }
            else
            {
                Xesti_pos=Xpos+err;
            }
            return Xesti_pos;
        }
        else
            {
                err=generror();
                Xesti_pos=Xpos;
                return Xesti_pos;
            }
    }
    if(!strcmp(rec, still))
    {
        err=generror();
        sign = subadd();
        err = err/2;
        if(sign==0&& Xpos!=0)
        {
            Xesti_pos=Xpos-err;
        }
        else
        {
            Xesti_pos=Xpos+err;
        }
        if (Xpos==0)
            {
                return Xpos;
            }
        else if(Xpos==10)
            {
                return Xpos;
            }
        else
            {
                return Xesti_pos;
            }
    }   
    if(!strcmp(rec, reset))
    {
        err=0;
        Xpos=0;
        Xesti_pos=Xpos+err;
        return Xesti_pos;
    }
}


int main(int argc, char * argv[]){
    printf("MotorZ: MotorZ succeded in execting\n");
    logfile();
    fprintf(log_file_poin, "MotorZ: MotorZ succeded in execting\n"); 
    fclose(log_file_poin);

    int fd2,fd3,watchdogPID; 
    
    // FIFO pipes opening
    char * commandZ = "/tmp/commandZ"; 
    char * inspectionz = "/tmp/inspectionz";
    mkfifo(inspectionz, 0666); 
    
    struct sigaction sig;
    memset(&sig, 0, sizeof(sig));
    sig.sa_handler= &handle_sigusr2;
    sigaction(SIGUSR2, &sig, NULL);

    while (1) 
    {
        createfileZ();
        logfile();
        fprintf(log_file_poin, "MotorZ: created text file to pass my PID\n");
        fclose(log_file_poin);
        fd2 = open(commandZ,O_RDONLY);
        fd_set rfds;
        struct timeval tv;
        int retval;
        FD_ZERO(&rfds);
        FD_SET(fd2, &rfds);
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        retval = select(fd2+1, &rfds, NULL, NULL, &tv);

        if (retval == -1)
        {
            perror("error!");
        }
        else if (retval)
        {
            /*
            data is now available
            */
           printf("MotorZ: recvied data from fifo\n");
           logfile();
           fprintf(log_file_poin, "MotorZ: recvied data from fifo\n");
           fclose(log_file_poin);
           read(fd2, rec, 80);
           logfile();
           fprintf(log_file_poin, "MotorZ: data recived is:");
           fprintf(log_file_poin,"%s\n", rec);
           fclose(log_file_poin);
           fd3 = open(inspectionz,O_WRONLY);
           if (rec[0] == 'q')
           {
               sen[0]='q';
               write(fd3, sen, strlen(sen)+1);
               logfile();
               fprintf(log_file_poin, "MotorZ: Received q terminate the process\n");
               fclose(log_file_poin);
               exit(EXIT_SUCCESS) ;
           }
           pos=motion();
           sprintf(sen, "%f", pos); //from string to float
    
           write(fd3, sen, strlen(sen)+1);
        }
        else
        {
            /*
            no data update
            */
           pos=motion();
           sprintf(sen, "%f", pos);
           fd3 = open(inspectionz,O_WRONLY);
           write(fd3, sen, strlen(sen)+1);
           logfile();
           fprintf(log_file_poin, "MotorZ: Send the position to inspection which is");
           fclose(log_file_poin);
        }
        watchdogPID = atoi(argv[1]);
        //send a signal to the watchdog
        kill(watchdogPID, SIGUSR1);
        logfile();
        fprintf(log_file_poin, "MotorZ: Send signal to watchdog");
        fclose(log_file_poin); 
        
    } 
    return 0; 
} 