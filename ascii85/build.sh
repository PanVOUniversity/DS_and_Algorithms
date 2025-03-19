#!/bin/bash

set -e

# Compile the main program
echo "Compiling main program..."
g++ -o ascii85 main.cpp ascii85.cpp -lstdc++