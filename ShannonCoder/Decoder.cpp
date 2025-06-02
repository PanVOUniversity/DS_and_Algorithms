#include "ShannonCoding.h"

bool decodeFile(const std::string& compressedFile, const std::string& dictionaryFile, const std::string& decodedFile) {
    std::map<std::string, unsigned char> decodingMap;
    if (!readDictionary(dictionaryFile, decodingMap)) {
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

