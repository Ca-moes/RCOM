#!/bin/bash

gnome-terminal --window-with-profile=RCOM -- ./run_read.bash
gnome-terminal --window-with-profile=RCOM_write -- ./run_write.bash
