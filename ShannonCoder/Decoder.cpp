#include "ShannonCoding.h"

bool decodeFile(const std::string& compressedFile, const std::string& dictionaryFile, const std::string& decodedFile) {
    std::map<std::string, unsigned char> decodingMap;
    if (!readDictionary(dictionaryFile, decodingMap)) {
        std::ifstream compressedCheckFile(compressedFile, std::ios::binary | std::ios::ate);
        if (compressedCheckFile.is_open() && compressedCheckFile.tellg() == sizeof(int)) {
            compressedCheckFile.seekg(0);
            int paddingBits;
            compressedCheckFile.read(reinterpret_cast<char*>(&paddingBits), sizeof(paddingBits));
            if (paddingBits == 0) {
                std::ofstream emptyDecodedFile(decodedFile, std::ios::binary);
                if (!emptyDecodedFile.is_open()) {
                    std::cerr << "Oopsie: Couldn't create empty decoded file: " << decodedFile << std::endl;
                    return false;
                }
                emptyDecodedFile.close();
                std::cout << "Heads up: Compressed file was empty. Created empty decoded file." << std::endl;
                return true;
            }
        }
        std::cerr << "Oopsie: Dictionary read failed and compressed file is not empty." << std::endl;
        return false;
    }

    std::ifstream inFile(compressedFile, std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << "Oopsie: Couldn't open compressed file: " << compressedFile << std::endl;
        return false;
    }

    std::ofstream outFile(decodedFile, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Oopsie: Couldn't open decoded file for writing: " << decodedFile << std::endl;
        inFile.close();
        return false;
    }

    int paddingBits = 0;
    inFile.read(reinterpret_cast<char*>(&paddingBits), sizeof(paddingBits));

    if (inFile.peek() == EOF && inFile.gcount() == 0) {
        std::cout << "Heads up: Compressed file contained only padding info. Decoded file is empty." << std::endl;
        inFile.close();
        outFile.close();
        return true;
    }

    std::string currentCode = "";
    unsigned char byte;
    
    while (inFile.read(reinterpret_cast<char*>(&byte), 1)) {
        std::string byteBits;
        for (int i = 7; i >= 0; --i) {
            byteBits += ((byte >> i) & 1) ? '1' : '0';
        }

        if (inFile.peek() == EOF) {
            byteBits = byteBits.substr(0, 8 - paddingBits);
        }

        for (char bit : byteBits) {
            currentCode += bit;
            auto it = decodingMap.find(currentCode);
            if (it != decodingMap.end()) {
                outFile.write(reinterpret_cast<const char*>(&it->second), 1);
                currentCode = "";
            }
        }
    }

    inFile.close();
    outFile.close();

    std::cout << "File decoded successfully. Decoded file: " << decodedFile << std::endl;

    return true;
}

