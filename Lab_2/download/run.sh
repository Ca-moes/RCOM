#!/bin/sh

clear
if gcc -o download -Wall -g download.c args.c connection.c; then 
./download ftp://ftp.up.pt/pub/gnu/GNUinfo/Audio/index.txt;
# ./download ftp://rcom:rcom@netlab1.fe.up.pt/pub.txt;
else 
echo "COMPILATION ERROR";
fi 

rm download

# user: rcom
# pass: rcom

#./download ftp://ftp.up.pt/pub/gnu/GNUinfo/Audio/index.txt;
#./download ftp://rcom:rcom@netlab1.fe.up.pt/pub.txt;
# não encontra file pub com login
