#!/bin/bash

# Compile C++ files and link with Google Test
g++ -o shannon_codec main.cpp Encoder.cpp Decoder.cpp ShannonCoding.cpp -std=c++17 -I/usr/include/gtest -I/usr/local/include/gtest -L/usr/lib -L/usr/local/lib -lgtest -lgtest_main -pthread

if [ $? -eq 0 ]; then
    echo "Compilation finished successfully. Executable: shannon_codec"
else
    echo "Compilation error."
    exit 1
fi

