#!/bin/bash

# sudo socat -d  -d  PTY,link=/dev/ttyS10,mode=777   PTY,link=/dev/ttyS11,mode=777

if gcc -Wall -o write writenoncanonical.c; then 
./write /dev/ttyS11;
else 
echo "COMPILATION ERROR";
fi 

rm write