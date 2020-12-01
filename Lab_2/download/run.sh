#!/bin/sh

clear
if gcc -o download -Wall -g download.c args.c connection.c; then 
./download ftp://ftp.up.pt/pub/gnu/GNUinfo/Audio/index.txt;
else 
echo "COMPILATION ERROR";
fi 

rm download

# user: rcom
# pass: rcom