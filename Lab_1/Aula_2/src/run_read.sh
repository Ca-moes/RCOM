#!/bin/bash

# sudo socat -d  -d  PTY,link=/dev/ttyS10,mode=777   PTY,link=/dev/ttyS11,mode=777

if gcc -o readbin noncanonical.c macros.h; then 
./readbin /dev/ttyS10;
else 
echo "COMPILATION ERROR";
fi 

rm readbin