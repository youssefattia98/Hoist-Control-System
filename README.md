The program is meant to move an hoist through the input of commands in 2 different consoles: one for controlling the direction of the movement of the hoist (Command console), the other one (Inspection console) shows the output of the motors and can send 2 emergency functions, STOP to immediately stop the movement of the motors and gives the opportunity to restart them later, RESET to immediately stop the movement of the hoist and bring the motors to the zero position.

The project is composed by 5 processes:

* master.c is the process which creates all the processes using the fork() function and executes them through an execvp() function that points to the files containing the other processes. The master process is the father of all the other ones, so we implemented the watchdog timer function in this process. The watchdog has a global boolean variable commandsig=true, receives a signal every time one of the processes is executed and through the sig_handler(int signo) function changes the value of this variable in false, if none of the process send a signal in 60 seconds, the watchdog timer will terminate all 4 the processes, otherwise it will continue to restart itself.

* Command.c this is the main control console. In this process the motors process IDs is read from their .txt files. The communications between this console and the motors is initialized with 2 independent named FIFO pipes. In an infinite while loop the process asks for the moving commands which are: j increase motorX position, l decrease motorZ position, i increase motorZ position, k decrease motorZ position, q terminates all the four processes; if the user presses any other key the motor will be still in the same position until another command is given. Here is implemented the signal handling to make the motor processes restart from the interrupted position once the process Inspection sent the signal STOP.

* Inspection.c this is the console used as User Interface, it prints -nan when none of the processes is working yet and needs and interaction from the Command console, after that shows to the user the changes of motors positions in the int main() function, and sends the emergency commands STOP and RESET in the father process section. The motors process IDs is read from their .txt files and then prints the motors position including an error .

* MotorX.c with createfileX() a .txt file is created in runtime to store the right motorxPID so that the signals from the other consoles can reach the right process. The function float motion() controls the movement of the motor, and sends it to the inspection console, in this function we recall the float generror() to implement the real situation of bias errors or measurement errors. The signal sent by the inspection process is managed by the void handel_sigurs2(int sig) function. 

* MotorZ is the equivalent of the MotorX.c process but for the Z positions. The functions implemented and the variables used are the same.

Each of this process has a section in which a signal is sent to the watchdog timer to prove theyre functioning.
In the repository theres the run.sh file that compiles and run all the processes at the same time on the Konsole terminal. 

Work done in cooperation with [Alice Maria Catalano](https://github.com/AliceCatalano) and [Hussein Hassan](https://github.com/husseinfd97).
