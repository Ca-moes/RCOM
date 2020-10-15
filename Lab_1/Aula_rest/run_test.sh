#!/bin/sh

gnome-terminal --window-with-profile=RCOM_soc -- sudo ./cable
cd src
sleep 5
./run_rw.sh
