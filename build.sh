#/bin/bash
mkdir build > /dev/null
cd build 
make clean > /dev/null
qmake ..
make
