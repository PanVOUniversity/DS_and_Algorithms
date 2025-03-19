#include <gtest/gtest.h>
#include "ascii85.h"

// Test encoding a block of 4 bytes
TEST(ASCII85Test, EncodeBlock) {
    std::vector<uint8_t> block = {72, 101, 108, 108}; // "Hell"
    std::string encoded = encodeBlock(block);
    EXPECT_EQ(encoded, "87cUR");
}

// Test decoding a block of 5 ASCII85 characters
TEST(ASCII85Test, DecodeBlock) {
    std::string block = "87cUR";
    std::vector<uint8_t> decoded = decodeASCII85Block(block);
    std::vector<uint8_t> expected = {72, 101, 108, 108}; // "Hell"
    EXPECT_EQ(decoded, expected);
}

// Test encoding and decoding a full string
TEST(ASCII85Test, EncodeDecodeString) {
    std::string input = "Hello, ASCII85!";
    std::string encoded;
    for (size_t i = 0; i < input.length(); i += 4) {
        std::vector<uint8_t> block;
        for (size_t j = 0; j < 4; j++) {
            if (i + j < input.length()) {
                block.push_back(static_cast<uint8_t>(input[i + j]));
            } else {
                block.push_back(0); // Padding
            }
        }
        encoded += encodeBlock(block);
    }

    std::string decoded;
    for (size_t i = 0; i < encoded.length(); i += 5) {
        std::string block = encoded.substr(i, 5);
        if (block.length() < 5) {
            block.append(5 - block.length(), 'u'); // Padding
        }
        std::vector<uint8_t> decodedBlock = decodeASCII85Block(block);
        // Remove padding bytes if necessary
        if (i + 5 >= encoded.length()) {
            size_t padding = 4 - (input.length() % 4);
            if (padding != 4) {
                decodedBlock.resize(decodedBlock.size() - padding);
            }
        }
        decoded.append(decodedBlock.begin(), decodedBlock.end());
    }

    EXPECT_EQ(decoded, input);
}



// Test invalid block size for decoding
TEST(ASCII85Test, InvalidBlockSize) {
    std::string block = "87ce"; // Less than 5 characters
    EXPECT_THROW(decodeASCII85Block(block), std::invalid_argument);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}