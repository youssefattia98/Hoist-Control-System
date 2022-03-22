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
