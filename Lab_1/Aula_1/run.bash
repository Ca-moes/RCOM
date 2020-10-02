#!/bin/bash
# run as "sudo ./run.bash"

gnome-terminal --window-with-profile=RCOM -- sudo socat -d  -d  PTY,link=/dev/ttyS10,mode=777   PTY,link=/dev/ttyS11,mode=777
sleep 5
gnome-terminal --window-with-profile=RCOM -- ./run_read.bash
gnome-terminal --window-with-profile=RCOM -- ./run_write.bash
