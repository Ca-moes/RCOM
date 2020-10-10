#!/bin/sh

gnome-terminal --window-with-profile=RCOM_soc -- sudo socat -d  -d  PTY,link=/dev/ttyS10,mode=777   PTY,link=/dev/ttyS11,mode=777
sleep 5
cd src
./run_rw.sh
