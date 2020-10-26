#!/bin/bash

clear
if gcc -o progwrite -Wall application.c pla.c logs.c statemachine.c; then 
echo "---------"
./progwrite transmitter pinguim.gif 10;
else 
echo "COMPILATION ERROR";
fi 

rm progwrite