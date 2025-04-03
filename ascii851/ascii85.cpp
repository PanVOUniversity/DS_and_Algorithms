#include "ascii85.h"
#include <algorithm> // For std::reverse

std::string encodeBlock(const std::vector<uint8_t>& block) {
    if (block.size() != 4) {
        throw std::invalid_argument("Block size must be 4 bytes for encoding");
    }

    uint32_t value = (block[0] << 24) | (block[1] << 16) | (block[2] << 8) | block[3];
    if (value == 0) {
        return "z";
    }

    std::string encoded;
    for (int i = 4; i >= 0; --i) {
        encoded += static_cast<char>((value % 85) + '!');
        value /= 85;
    }

    std::reverse(encoded.begin(), encoded.end());
    return encoded;
}

std::vector<uint8_t> decodeASCII85Block(const std::string& block) {
    if (block.length() != 5) {
        throw std::invalid_argument("Block size must be 5 characters for decoding");
    }

    if (block == "z") {
        return {0, 0, 0, 0};
    }

    uint32_t value = 0;
    for (char c : block) {
        value = value * 85 + decodeChar(c);
    }

    std::vector<uint8_t> decoded;
    decoded.push_back(static_cast<uint8_t>((value >> 24) & 0xFF));
    decoded.push_back(static_cast<uint8_t>((value >> 16) & 0xFF));
    decoded.push_back(static_cast<uint8_t>((value >> 8) & 0xFF));
    decoded.push_back(static_cast<uint8_t>(value & 0xFF));
    return decoded;
}

uint8_t decodeChar(char c) {
    if (c < '!' || c > 'u') {
        throw std::invalid_argument("Invalid ASCII85 symbol");
    }

    return static_cast<uint8_t>(c - '!');
}