#!/bin/bash
set -e

EIGEN_INC="/usr/include/eigen3"
GTEST_INC="/usr/include/gtest"
GTEST_LIB="-lgtest -lpthread"

# Compile solver

echo "Compiling solver..."
g++ -I "$EIGEN_INC" -o solver solver.cpp csv_io.cpp

echo "Compiling system generator..."
g++ -o generate_system generate_system.cpp

echo "Compiling tests (gtest)..."
g++ -I "$EIGEN_INC" -I "$GTEST_INC" -o test_solver test_solver.cpp csv_io.cpp $GTEST_LIB

echo "Build complete." 