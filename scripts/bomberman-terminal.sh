#!/usr/bin/env bash

# GOTO THIS SCRIPT
cd $(dirname ${BASH_SOURCE[0]})

# CHECK FOR PIO FILE
cd ..
if [ ! -f platformio.ini ]; then
    echo "No platformio.ini file found in directory under this script."
    exit 1
fi

# UPLOAD MOST RECENT PROGRAM
pio run -t upload

if [ ! $? ]; then
    echo "Recent program does not compile."
    exit 1
fi

clear

pio device monitor -b 115200 --raw

clear
