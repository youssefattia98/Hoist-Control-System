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
char sen[80]="0";

float pos=0;
int sign=0;

char senstr[100];
int motorxPID;

//keys
char inc[] = "Inc";
char dec[] = "Dec";
char still[] = "Sti";
char reset[] ="res";

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

void createfileX(){
    /*
        This function creates and writes in a file to store the Motor Z process ids
    */
    FILE *fp;

    // Open file in write mode
    fp = fopen("./../Textfiles/pidMotorX","w+");

    // If file opened successfully,  write the string to file
    if ( fp )
    {
       motorxPID=getpid();
        sprintf(senstr, "%d", motorxPID);
        logfile();
        fprintf(log_file_poin, "MotorX: The file created to store PID of motorx which is");
        fprintf(log_file_poin, "%s\n",senstr);
        fclose(log_file_poin);
	    fputs(senstr,fp);
    }
    else
        {
            printf("Failed to open the file\n");
            logfile();
            fprintf(log_file_poin, "Motorx: Faild to open the file");
            fclose(log_file_poin);
        }
    //Close the file
    fclose(fp);
}

float generror(){
    /*
        This function returns a random number between -0.5 and 0.5 as float
    */
    int sign=0; 
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
    strcpy(rec,reset);
    logfile();
    fprintf(log_file_poin, "MotorX: The Reset signal is received\n");
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



int main(int argc, char * argv[]) 
{ 
    printf("MotorX: MotorX succeded in execting\n");
    logfile();
    fprintf(log_file_poin, "MotorX: MotorX succeded in execting\n"); 
    fclose(log_file_poin);

    int fd1,fd2,watchdogPID;

    // FIFO pipes opening
    char * commandX = "/tmp/commandX"; 
    char * inspectionx = "/tmp/inspectionx"; 
    mkfifo(inspectionx, 0666);

    struct sigaction sig;
    memset(&sig,0,sizeof(sig));
    sig.sa_handler=&handle_sigusr2;
    sigaction(SIGUSR2,&sig,NULL);
  
    while (1) 
    {
        createfileX();
        logfile();
        fprintf(log_file_poin, "MotorX: created text file to pass my PID\n");
        fclose(log_file_poin);
        fd1 = open(commandX,O_RDONLY);
        fd_set rfds;
        struct timeval tv;
        int retval;
        FD_ZERO(&rfds);
        FD_SET(fd1, &rfds);
        tv.tv_sec = 1;
        tv.tv_usec = 0;
        retval = select(fd1+1, &rfds, NULL, NULL, &tv);

        if (retval == -1)
        {
            perror("error!");
        }
        else if (retval)
        {
            /*
            data is now available
            */
           printf("MotorX: recvied data from fifo\n");
           logfile();
           fprintf(log_file_poin, "MotorX: recvied data from fifo\n");
           fclose(log_file_poin);
           read(fd1, rec, 80);
           logfile();
           fprintf(log_file_poin, "MotorX: data recived is:");
           fprintf(log_file_poin,"%s\n",rec);
           fclose(log_file_poin);
           fd2 = open(inspectionx,O_WRONLY);
           if (rec[0] == 'q')
           {
               sen[0]='q';  
               write(fd2, sen, strlen(sen)+1);
               logfile();
               fprintf(log_file_poin, "MotorX: Received q terminate the process\n");
               fclose(log_file_poin);
               exit(EXIT_SUCCESS) ;
           }
           pos=motion();
           sprintf(sen, "%f", pos);
           write(fd2, sen, strlen(sen)+1);
        }
        else
        {   /*
            no data update
            */
            fd2 = open(inspectionx,O_WRONLY);
            pos=motion();
            sprintf(sen, "%f", pos);
            write(fd2, sen, strlen(sen)+1);
            logfile();
            fprintf(log_file_poin, "MotorX: Send the position to inspection which is");
            fclose(log_file_poin);
        }
        watchdogPID = atoi(argv[1]);
        //send signal to watchdog
        kill(watchdogPID, SIGUSR1); 
        logfile();
        fprintf(log_file_poin, "MotorX: Send signal to watchdog");
        fclose(log_file_poin); 
    } 
    return 0; 
}