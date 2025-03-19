#ifndef ASCII85_H
#define ASCII85_H

#include <vector>
#include <string>
#include <stdexcept>
#include <cstdint>

// Function to encode a block of 4 bytes into ASCII85
std::string encodeBlock(const std::vector<uint8_t>& block);

// Function to decode a block of 5 ASCII85 characters into bytes
std::vector<uint8_t> decodeASCII85Block(const std::string& block);

// Function to decode a single ASCII85 character
uint8_t decodeChar(char c);

#endif // ASCII85_H