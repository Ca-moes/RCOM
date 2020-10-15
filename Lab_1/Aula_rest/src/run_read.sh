#!/bin/bash


if gcc -o readbin -Wall noncanonical.c macros.h; then 
./readbin /dev/ttyS10;
else 
echo "COMPILATION ERROR";
fi 

rm readbin