#!/usr/bin/env python3
import base64
import random
import subprocess
import sys
from typing import Tuple

def run_encoder(input_data: bytes, decode_mode: bool = False) -> Tuple[bytes, int]:
    """Run the C++ encoder/decoder and return its output and return code."""
    process = subprocess.Popen(
        ['./ascii85', '-d'] if decode_mode else ['./ascii85', '-e'],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )
    stdout, stderr = process.communicate(input=input_data)
    return stdout, process.returncode

def test_encoding():
    """Test encoding with random data against Python's a85encode."""
    for _ in range(100):
        # Generate random data of random length (1-256 bytes)
        length = random.randint(1, 256)
        data = bytes(random.getrandbits(8) for _ in range(length))
        
        # Encode with Python
        py_encoded = base64.a85encode(data, adobe=False)
        
        # Encode with C++
        cpp_encoded, return_code = run_encoder(data)
        
        # Check that C++ succeeded
        assert return_code == 0, f"Encoder failed with return code {return_code}"
        
        # Compare results
        assert py_encoded == cpp_encoded, \
            f"Encoding mismatch for input {data}\nPython: {py_encoded}\nC++:    {cpp_encoded}"

def test_decoding():
    """Test decoding with random data against Python's a85decode."""
    for _ in range(100):
        # Generate random data and encode it to get valid ASCII85
        length = random.randint(1, 256)
        data = bytes(random.getrandbits(8) for _ in range(length))
        encoded_data = base64.a85encode(data, adobe=False)
        
        # Decode with Python
        py_decoded = base64.a85decode(encoded_data, adobe=False)
        
        # Decode with C++
        cpp_decoded, return_code = run_encoder(encoded_data, decode_mode=True)
        
        # Check that C++ succeeded
        assert return_code == 0, f"Decoder failed with return code {return_code}"
        
        # Compare results
        print(f"Python decoded ({len(py_decoded)}): {repr(py_decoded)}")
        print(f"C++ decoded    ({len(cpp_decoded)}): {repr(cpp_decoded)}")
        assert py_decoded == cpp_decoded, \
            f"Decoding mismatch for input {encoded_data}\nPython: {py_decoded}\nC++:    {cpp_decoded}"



if __name__ == '__main__':
    # Build the C++ program first
    subprocess.run(['./build.sh'], check=True)
    
    # Run tests
    test_encoding()
    test_decoding()
 
    
    print("All Python comparison tests passed!")