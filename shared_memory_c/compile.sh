#!/bin/bash


gcc -Wall -Wno-write-strings -O2 main.c getopts.c shared.c -o memtest -lrt -lpthread || exit 1
gcc -Wall -Wno-write-strings -O2 -shared -fPIC -I. -I/usr/local/jdk1.7.0_55/include -I/usr/local/jdk1.7.0_55/include/linux shared.c jni/sharedmemory_SharedMemory.c -o libsharedmemory.so -lrt -lpthread || exit 1
cp libsharedmemory.so sharedmemory/
