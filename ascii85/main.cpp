#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <iomanip>
#include "ascii85.h"

int main(int argc, char* argv[])
{
    bool decodeMode = false;
    std::string input;

    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (arg == "-d")
        {
            decodeMode = true;
        }
        else if (arg == "-e")
        {
            decodeMode = false;
        }
        else
        {
            input = arg;
        }
    }

    if (input.empty())
    {
        char byte;
        while (std::cin.get(byte))
        {
            input.push_back(byte);
        }
    }

    try
    {
        std::string output;

        // In main.cpp, modify the decoding section:
        if (decodeMode)
        {
            for (size_t i = 0; i < input.length(); i += 5)
            {
                std::string block = input.substr(i, 5);
                if (block.length() < 5)
                {
                    block.append(5 - block.length(), 'u');
                }
                std::vector<uint8_t> decoded = decodeASCII85Block(block);

                // Only add the full 4 bytes if we're not at the last block
                if (i + 5 < input.length())
                {
                    output.append(decoded.begin(), decoded.end());
                }
                else
                {
                    // For the last block, calculate how many bytes we actually need
                    size_t original_length = input.length();
                    size_t full_blocks = original_length / 5;
                    size_t remainder = original_length % 5;

                    // Calculate how many bytes we should output
                    size_t bytes_to_output = (full_blocks * 4) +
                                             (remainder ? remainder - 1 : 0);
                    size_t current_length = output.length();

                    if (bytes_to_output > current_length)
                    {
                        size_t bytes_needed = bytes_to_output - current_length;
                        bytes_needed = std::min(bytes_needed, decoded.size());
                        output.append(decoded.begin(), decoded.begin() + bytes_needed);
                    }
                }
            }
        }
        else
        {
            size_t inputLength = input.length();
            size_t padding = (4 - (inputLength % 4)) % 4;
            input.append(padding, '\0');

            for (size_t i = 0; i < input.length(); i += 4)
            {
                std::vector<uint8_t> block;
                for (size_t j = 0; j < 4; j++)
                {
                    block.push_back(static_cast<uint8_t>(input[i + j]));
                }
                output += encodeBlock(block);
            }

            if (padding > 0)
            {
                output.erase(output.length() - padding, padding);
            }
        }

        std::cout << output;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}