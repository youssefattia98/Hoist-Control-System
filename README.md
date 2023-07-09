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

Work done in cooperation with [Alice Maria Catalano](https://github.com/AliceCatalano) and [Hussein Hassan](https://github.com/husseinfd97).
