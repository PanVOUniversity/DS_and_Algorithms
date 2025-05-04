#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include "ascii85.h"
#include <algorithm> // For std::min

int main(int argc, char* argv[]) {
    bool decodeMode = false;
    std::string input;

    // Process command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-d") {
            decodeMode = true;
        } else if (arg == "-e") {
            decodeMode = false;
        } else {
            // Assuming the last argument is the input string if not reading from stdin
            input = arg;
        }
    }

    // Read from stdin if no input string is provided via arguments
    if (input.empty()) {
        char byte;
        while (std::cin.get(byte)) {
            input.push_back(byte);
        }
    }

    try {
        std::string output;

        if (decodeMode) {
            // Check for invalid length % 5 == 1 at the beginning
            if (input.length() % 5 == 1) {
                std::cerr << "Error: Invalid ASCII85 input: length mod 5 == 1" << std::endl;
                return 1; // Indicate an error
            }

            size_t current_pos = 0;
            while (current_pos < input.length()) {
                size_t remaining_len = input.length() - current_pos;
                // Determine the size of the current block (5 for full blocks, remaining_len for the last block)
                size_t block_size = std::min((size_t)5, remaining_len);

                std::string block = input.substr(current_pos, block_size);

                std::vector<uint8_t> decoded_block;

                if (block == "z") {
                    // The 'z' shortcut can only appear as a single character representing 4 null bytes
                    if (block_size != 1) {
                         std::cerr << "Error: Invalid ASCII85 input: 'z' in a block of size " << block_size << std::endl;
                         return 1; // Indicate an error
                    }
                    decoded_block = {0, 0, 0, 0};
                } else {
                    // For the last partial block (size 2 to 4), pad with 'u' to make it 5 characters for decodeASCII85Block
                    // decodeASCII85Block expects 5 characters.
                    if (block_size < 5) {
                        // Invalid input: a partial block must be at least 2 characters
                        if (block_size < 2) {
                             std::cerr << "Error: Invalid ASCII85 input: partial block too short." << std::endl;
                             return 1; // Indicate an error
                        }
                        block.append(5 - block_size, 'u');
                    }

                    try {
                        decoded_block = decodeASCII85Block(block);
                    } catch (const std::invalid_argument& e) {
                        // Catch invalid characters within the block as thrown by decodeChar or decodeASCII85Block
                        std::cerr << "Error during block decoding: " << e.what() << std::endl;
                        return 1; // Indicate an error
                    }

                    // If it was a partial block, resize the decoded output to the correct number of bytes
                    // A block of size k (2 <= k <= 4) decodes to k-1 bytes of original data.
                    if (block_size < 5) {
                        decoded_block.resize(block_size - 1);
                    }
                     // If it was a full block (size 5), it decodes to 4 bytes, no resize needed here.
                }

                // Append the decoded bytes to the output
                output.append(decoded_block.begin(), decoded_block.end());
                // Move the current position forward by the size of the block just processed
                current_pos += block_size;
            }
        } else { // Encoding mode
            size_t inputLength = input.length();
            // Calculate padding needed to make input length a multiple of 4
            size_t padding = (4 - (inputLength % 4)) % 4;
            input.append(padding, '\0'); // Append null bytes for padding

            // Process input in blocks of 4 bytes
            for (size_t i = 0; i < input.length(); i += 4) {
                std::vector<uint8_t> block;
                for (size_t j = 0; j < 4; j++) {
                    block.push_back(static_cast<uint8_t>(input[i + j]));
                }
                output += encodeBlock(block);
            }

            // Remove the characters corresponding to padding from the end of the encoded output
            // The number of characters to remove is equal to the number of padding bytes added.
            if (padding > 0) {
                output.erase(output.length() - padding, padding);
            }
        }

        // Print the result to standard output
        std::cout << output;
    } catch (const std::exception& e) {
        // Catch any exceptions (like invalid_argument from decodeChar or decodeASCII85Block)
        std::cerr << "Error: " << e.what() << std::endl;
        return 1; // Indicate an error
    }

    return 0; // Indicate success
}
