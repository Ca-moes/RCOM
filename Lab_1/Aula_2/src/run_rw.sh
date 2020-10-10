#!/bin/bash
gnome-terminal --window-with-profile=RCOM_read -- ./run_read.sh
gnome-terminal --window-with-profile=RCOM_write -- ./run_write.sh
