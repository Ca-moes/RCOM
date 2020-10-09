#!/bin/bash

gnome-terminal --window-with-profile=RCOM -- ./run_read.bash
sleep 1
gnome-terminal --window-with-profile=RCOM -- ./run_write.bash
