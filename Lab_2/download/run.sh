#!/bin/sh

clear
if gcc -o download -Wall -g download.c args.c connection.c; then 
./download ftp://rcom:rcom@netlab1.fe.up.pt/files/pic2.png;
else 
echo "COMPILATION ERROR";
fi 

rm download

## Funcionam todos os que estão abaixo:

#./download ftp://ftp.up.pt/pub/gnu/GNUinfo/Audio/index.txt;

#./download ftp://netlab1.fe.up.pt/pub.txt;

#./download ftp://rcom:rcom@netlab1.fe.up.pt/pipe.txt;
#./download ftp://rcom:rcom@netlab1.fe.up.pt/files/pic1.jpg;
#./download ftp://rcom:rcom@netlab1.fe.up.pt/files/pic2.png;
