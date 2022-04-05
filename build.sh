#!/bin/sh

if [ -d build ]; then
    cd build
    make
    cd ..
else
    mkdir build
    cmake -S src -B build
    cd build
    make
    cd ..
fi
