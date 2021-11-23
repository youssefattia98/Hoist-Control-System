gcc Command.c -o command
gcc MotorX.c -o motorx
gcc MotorZ.c -o motorz
gcc Inspection.c -o inspection

konsole  -e ./command &
konsole  -e ./motorx &
konsole  -e ./motorz &
konsole  -e ./inspection &
