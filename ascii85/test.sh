#!/bin/bash

set -e

# Compile the unit tests with GoogleTest
echo "Compiling unit tests..."
g++ -o test_ascii85 test.cpp ascii85.cpp -lgtest -lgtest_main -lpthread -lstdc++

# Run unit tests
echo "Running unit tests..."
./test_ascii85

# Run random data tests using Python
echo "Running random data tests..."
python3 test_random.py

echo "All tests passed!"