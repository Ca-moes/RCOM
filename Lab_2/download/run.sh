#!/bin/sh

clear
if gcc -o download -Wall -g download.c args.c; then 
./download ftp://abcd111111111111:12345@netlab1.fe.up.pt/pub.txt;
else 
echo "COMPILATION ERROR";
fi 

rm download

# user: rcom
# pass: rcom