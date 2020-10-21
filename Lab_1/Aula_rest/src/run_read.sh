#!/bin/bash


if gcc -o progread -Wall application.c pla.c logs.c; then
echo "---------"
./progread receiver 11;
else 
echo "COMPILATION ERROR";
fi 

rm progread