#!/bin/bash

clear

if gcc -o progread -Wall -g application.c dcp.c dcp_spec.c logs.c statemachine.c; then
echo "---------"
valgrind --leak-check=yes ./progread receiver ../docs/ 11;
else 
echo "COMPILATION ERROR";
fi 

rm progread