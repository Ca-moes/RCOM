#!/bin/sh

# corre num novo terminal o socat
gnome-terminal --window-with-profile=RCOM_soc -- sudo ./cable
# tempo para dar input
sleep 5

cd src
make -s
if [ $? -eq 0 ] ; then
  gnome-terminal --window-with-profile=RCOM_read -- ./noncanonical /dev/ttyS10
  gnome-terminal --window-with-profile=RCOM_write -- ./writenoncanonical /dev/ttyS11
else
  echo "MAKE ERROR";
fi

sleep 0.5s
make clean