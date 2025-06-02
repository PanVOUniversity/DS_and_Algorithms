# **Shannon Encoder (C++)**

This project implements a basic Shannon encoder and decoder for binary files. It calculates character frequencies, generates Shannon-Fano-like prefix codes, compresses the input file, and provides a decoder to reconstruct the original data. The project also includes automated tests to verify the integrity of the encoding and decoding process.

## **Features**

* **Encoder (Encoder.cpp):**  
  * Analyzes input binary files to determine byte frequencies.  
  * Generates variable-length Shannon-Fano-like prefix codes.  
  * Writes the compressed data to an output file.  
  * Saves the generated dictionary (mapping bytes to codes) to a separate file.  
* **Decoder (Decoder.cpp):**  
  * Reads the compressed file and the corresponding dictionary file.  
  * Reconstructs the original uncompressed data.  
  * Writes the decoded data to an output file.  
* **Tests (main.cpp):**  
  * Includes a runTest function that creates various test files (text, binary, empty, single character, repeated character).  
  * Automatically encodes, decodes, and compares the original and decoded files to ensure correctness.  
  * Cleans up temporary test files after each run.

## **Project Structure**

.  
├── build.sh            \# Script to compile the C++ source files  
├── main.cpp            \# Main program, includes test cases  
├── Encoder.cpp         \# Implements the file encoding logic  
├── Decoder.cpp         \# Implements the file decoding logic  
└── ShannonCoding.h     \# Defines common data structures and code generation function

## **How to Build**

To compile the project, navigate to the project's root directory in your terminal and run the build.sh script. This script uses g++ to compile the source files.

1. **Make the script executable:**  
   chmod \+x build.sh

2. **Run the build script:**  
   ./build.sh

   You should see a message indicating successful compilation and the creation of the shannon\_codec executable.

## **How to Run**

After successful compilation, you can run the executable. The main.cpp file is set up to automatically run a series of tests.

1. **Execute the compiled program:**  
   ./shannon\_codec

   The program will output messages detailing each test case, including file creation, encoding, decoding, and comparison results. It will also indicate whether each test passed or failed.

## **Notes**

* This implementation uses a simplified Shannon-Fano-like algorithm for code generation, which ensures prefix codes for correct decoding.  
* The dictionary format is custom binary.  
* The compressed file stores the number of padding bits at the beginning to correctly handle the last byte during decoding.