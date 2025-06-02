#include "ShannonCoding.h"

bool encodeFile(const std::string& inputFile, const std::string& compressedFile, const std::string& dictionaryFile) {
    std::ifstream inFile(inputFile, std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << "Oopsie: Couldn't open input file: " << inputFile << std::endl;
        return false;
    }

    std::map<unsigned char, int> frequencies;
    unsigned char byte;
    while (inFile.read(reinterpret_cast<char*>(&byte), 1)) {
        frequencies[byte]++;
    }
    inFile.close();

    if (frequencies.empty()) {
        std::ofstream emptyCompressedFile(compressedFile, std::ios::binary);
        if (!emptyCompressedFile.is_open()) {
            std::cerr << "Oopsie: Couldn't create empty compressed file: " << compressedFile << std::endl;
            return false;
        }
        int paddingBits = 0;
        emptyCompressedFile.write(reinterpret_cast<const char*>(&paddingBits), sizeof(paddingBits));
        emptyCompressedFile.close();

        std::map<unsigned char, std::string> emptyEncodingMap;
        if (!writeDictionary(dictionaryFile, emptyEncodingMap)) {
            return false;
        }
        std::cout << "Heads up: Input file was empty. Created empty compressed file and dictionary." << std::endl;
        return true;
    }

    std::vector<CodeInfo> codes;
    for (const auto& pair : frequencies) {
        codes.push_back({pair.first, pair.second, ""});
    }

    std::sort(codes.begin(), codes.end(), std::greater<CodeInfo>());

    generateCodes(codes, 0, codes.size() - 1, "");

    std::map<unsigned char, std::string> encodingMap;
    for (const auto& info : codes) {
        encodingMap[info.byte] = info.code;
    }

    if (!writeDictionary(dictionaryFile, encodingMap)) {
        return false;
    }

    std::ofstream outFile(compressedFile, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Oopsie: Couldn't open compressed file for writing: " << compressedFile << std::endl;
        return false;
    }

    int placeholderPaddingBits = 0;
    outFile.write(reinterpret_cast<const char*>(&placeholderPaddingBits), sizeof(placeholderPaddingBits));


    inFile.open(inputFile, std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << "Oopsie: Couldn't re-open input file: " << inputFile << std::endl;
        outFile.close();
        return false;
    }

    unsigned char currentByte = 0;
    int bitCount = 0;
    long long totalBits = 0;

    while (inFile.read(reinterpret_cast<char*>(&byte), 1)) {
        std::string code = encodingMap[byte];
        totalBits += code.length();

        for (char bitChar : code) {
            currentByte = (currentByte << 1) | (bitChar == '1' ? 1 : 0);
            bitCount++;
            if (bitCount == 8) {
                outFile.write(reinterpret_cast<const char*>(&currentByte), 1);
                currentByte = 0;
                bitCount = 0;
            }
        }
    }

    int paddingBits = 0;
    if (bitCount > 0) {
        paddingBits = 8 - bitCount;
        currentByte = currentByte << paddingBits;
        outFile.write(reinterpret_cast<const char*>(&currentByte), 1);
    }
    
    outFile.seekp(0);
    outFile.write(reinterpret_cast<const char*>(&paddingBits), sizeof(paddingBits));

    inFile.close();
    outFile.close();

    std::cout << "File encoded successfully. Compressed file: " << compressedFile
              << ", Dictionary: " << dictionaryFile << std::endl;
    std::cout << "Total bits in compressed file (before padding): " << totalBits << std::endl;

    return true;
}

