#!/bin/bash

if [ ! -d "build/" ]; then
        mkdir build
fi
g++ main.cpp -o build/snake
