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
        std::cerr << "Oopsie: Input file is empty or has no data." << std::endl;
        return false;
    }

    std::vector<CodeInfo> codes;
    for (const auto& pair : frequencies) {
        codes.push_back({pair.first, pair.second, ""});
    }

    std::sort(codes.begin(), codes.end(), std::greater<CodeInfo>());

    if (!codes.empty()) {
        generateCodes(codes, 0, codes.size() - 1, "");
    } else {
        std::cerr << "Heads up: No characters to encode." << std::endl;
        return false;
    }

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

    inFile.open(inputFile, std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << "Oopsie: Couldn't re-open input file: " << inputFile << std::endl;
        outFile.close();
        return false;
    }

    std::string bitBuffer = "";
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

