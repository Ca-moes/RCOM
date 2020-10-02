#!/bin/bash

# sudo socat -d  -d  PTY,link=/dev/ttyS10,mode=777   PTY,link=/dev/ttyS11,mode=777

if gcc -o read_ noncanonical.c; then 
./read_ /dev/ttyS11;
else 
echo "COMPILATION ERROR";
fi 

