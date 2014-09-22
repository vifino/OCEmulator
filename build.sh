#!/bin/bash
# Runs the commands to build OCEmulator
mkdir build &> /dev/null
cd build 
make clean &> /dev/null
cmake ..
make
