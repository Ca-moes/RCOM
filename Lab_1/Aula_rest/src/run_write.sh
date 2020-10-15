#!/bin/bash


if gcc -o progwrite -Wall application.c pla.c logs.c; then 
./progwrite transmitter 11;
else 
echo "COMPILATION ERROR";
fi 

rm progwrite