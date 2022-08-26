#!/bin/sh

if [ -d build ]; then
    cd build
    make
    cd ..
else
    cmake -S . -B build
    cd build
    make
    cd ..
fi
