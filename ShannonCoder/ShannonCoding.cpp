#include "ShannonCoding.h"

void generateCodes(std::vector<CodeInfo>& codes, int start, int end, const std::string& currentPrefix) {
    if (start == end) {
        codes[start].code = currentPrefix;
        return;
    }

    if (start >= end) {
        return;
    }

    long long totalFreq = 0;
    for (int i = start; i <= end; ++i) {
        totalFreq += codes[i].frequency;
    }

    long long runningFreq = 0;
    int splitPoint = start;
    for (int i = start; i <= end; ++i) {
        runningFreq += codes[i].frequency;
        if (runningFreq >= totalFreq / 2) {
            splitPoint = i;
            break;
        }
    }

    if (splitPoint == end && start != end) {
        splitPoint = end - 1;
    }
    if (splitPoint == start && start != end) {
    }


    generateCodes(codes, start, splitPoint, currentPrefix + '0');
    generateCodes(codes, splitPoint + 1, end, currentPrefix + '1');
}

bool writeDictionary(const std::string& dictFileName, const std::map<unsigned char, std::string>& codeMap) {
    std::ofstream outFile(dictFileName, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Oopsie: Couldn't open dictionary file for writing: " << dictFileName << std::endl;
        return false;
    }

    size_t howManyEntries = codeMap.size();
    outFile.write(reinterpret_cast<const char*>(&howManyEntries), sizeof(howManyEntries));

    for (const auto& entry : codeMap) {
        outFile.write(reinterpret_cast<const char*>(&entry.first), sizeof(entry.first));
        size_t codeLen = entry.second.length();
        outFile.write(reinterpret_cast<const char*>(&codeLen), sizeof(codeLen));
        outFile.write(entry.second.c_str(), codeLen);
    }

    outFile.close();
    return true;
}

bool readDictionary(const std::string& dictFileName, std::map<std::string, unsigned char>& decodedMap) {
    std::ifstream inFile(dictFileName, std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << "Oopsie: Couldn't open dictionary file for reading: " << dictFileName << std::endl;
        return false;
    }

    size_t howManyEntries;
    inFile.read(reinterpret_cast<char*>(&howManyEntries), sizeof(howManyEntries));

    for (size_t i = 0; i < howManyEntries; ++i) {
        unsigned char theByte;
        size_t codeLen;
        inFile.read(reinterpret_cast<char*>(&theByte), sizeof(theByte));
        inFile.read(reinterpret_cast<char*>(&codeLen), sizeof(codeLen));

        std::string theCode(codeLen, '\0');
        inFile.read(&theCode[0], codeLen);
        decodedMap[theCode] = theByte;
    }

    inFile.close();
    return true;
}

