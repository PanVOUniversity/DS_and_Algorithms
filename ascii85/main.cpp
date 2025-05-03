#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include "ascii85.h"

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
            // --- Added check for invalid input lengths ---
            // ASCII85 encoded data (without 'z') should have a length that is a multiple of 5,
            // or 1 more than a multiple of 5 (for the last partial block).
            // This check specifically targets lengths that don't fit this pattern and are not empty.
            // The case where length % 5 == 1 is handled by the existing throw statement.
            if (input.length() > 0 && input.length() % 5 != 0 && input.length() % 5 != 1) {
                 std::cerr << "Error: Invalid ASCII85 input length." << std::endl;
                 return 1; // Indicate an error as per requirements
            }
            // --- End of added check ---

            // Existing check for length % 5 == 1
            if (input.length() % 5 == 1) {
                throw std::runtime_error("Invalid ASCII85 input: length mod 5 == 1");
            }

            size_t num_blocks = input.length() / 5;
            size_t last_block_len = input.length() % 5;
            // Calculate expected decoded length, accounting for partial last block
            size_t decoded_expected_length = num_blocks * 4;
            if (last_block_len != 0) {
                decoded_expected_length += last_block_len - 1;
            }

            // Process input in blocks of 5 characters
            for (size_t i = 0; i < input.length(); i += 5) {
                std::string block = input.substr(i, 5);
                // Pad the last block with 'u' if it's shorter than 5 (shouldn't happen with valid input except the very end)
                if (block.length() < 5) {
                    block.append(5 - block.length(), 'u');
                }
                std::vector<uint8_t> decoded = decodeASCII85Block(block);

                // Resize the last decoded block to remove padding bytes if necessary
                if (i + 5 >= input.length() && last_block_len != 0) {
                    decoded.resize(last_block_len - 1);
                }
                output.append(decoded.begin(), decoded.end());
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
