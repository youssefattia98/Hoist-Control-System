# Hoist Control System

The program is designed to control a hoist using inputs from two different consoles. The system can control the direction of hoist movement (Command console) and provide outputs from the motors (Inspection console). In addition, it offers two emergency functions: 

- **STOP**: Immediately halt the movement of the motors, with the option to restart later.
- **RESET**: Immediately halt the hoist movement and return the motors to the zero position.

## System Architecture

The project consists of five processes:

1. `master.c` creates all the other processes using the `fork()` function. It executes them using an `execvp()` function that points to the files containing the other processes. The master process is the parent of all others, hence it includes the watchdog timer function.

2. `Command.c` serves as the main control console. It reads the motors’ process IDs from their .txt files. Communication between this console and the motors is established using two independent named FIFO pipes.

3. `Inspection.c` is the console used as the user interface. It prints “-nan” when none of the processes are working yet, and it requires an interaction from the Command console. After receiving the interaction, it shows the changes in the motors' positions to the user. It also sends the emergency commands STOP and RESET in the parent process section.

4. `MotorX.c` and `MotorZ.c` control the X and Z positions, respectively. They both use the same implemented functions and variables. Each of these processes sends a signal to the watchdog timer to verify they’re functioning.

## Installation

Use the `install.sh` script to compile and install the application:

```bash
#!/bin/bash
echo $1
unzip ZippedHA_1.zip -d $1
cd $1/HA_1
cd Command
gcc Command.c -o command
cd ..
cd MotorX
gcc MotorX.c -o motorx
cd ..
cd MotorZ
gcc MotorZ.c -o motorz
cd ..
cd Inspection
gcc Inspection.c -o inspection
cd ..
cd Master
gcc master.c -o masterWD
```
## Running the Application
Use the run.sh script to start the application:
```bash
#!/bin/bash
echo please enter path of the unzipped folder
read pathname
cd $pathname/HA_1/Master
./masterWD
```

## System Interaction

After running the application using the `run.sh` script, the system waits for inputs from the Command console for hoist movement. The commands for movement are as follows:

- `j` : Increase motorX position
- `l` : Decrease motorZ position
- `i` : Increase motorZ position
- `k` : Decrease motorZ position
- `q` : Terminate all the four processes

For any other key press, the motor will remain in the same position until another valid command is given.

## System Resilience

The system employs a watchdog mechanism in the master process to monitor the functioning of all subprocesses. The watchdog timer relies on a global boolean variable `commandsig` initially set to `true`. Each subprocess sends a signal to the master process upon successful execution, causing the watchdog timer to reset `commandsig` to `false`. If none of the subprocesses send a signal within a 60-second window, the watchdog timer will terminate all subprocesses, thus ensuring the overall system stability.

## Emergency Controls

In case of emergency, the system provides two functions - STOP and RESET. These functions can be accessed from the Inspection console. The STOP command halts all motor movements but allows for later resumption. The RESET command not only stops all movement but also brings the motors to a zero position.

## Important Note

All of the above processes send a signal to the watchdog timer to prove they’re functioning. Additionally, each motor process writes its process ID to a .txt file at runtime. This allows for correct signal routing from the consoles to the intended processes.

For more details about the implementation of the individual processes, please refer to the source code documentation.
Work done in cooperation with [Alice Maria Catalano](https://github.com/AliceCatalano) and [Hussein Hassan](https://github.com/husseinfd97).
