#!/bin/bash

clear

if gcc -o progread -Wall application.c pla.c logs.c statemachine.c; then
echo "---------"
./progread receiver ../docs/ 11;
else 
echo "COMPILATION ERROR";
fi 

rm progread