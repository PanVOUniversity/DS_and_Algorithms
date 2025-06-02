#ifndef SHANNON_CODING_H
#define SHANNON_CODING_H

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>

struct CodeInfo {
    unsigned char byte;
    int frequency;
    std::string code;

    bool operator>(const CodeInfo& other) const {
        return frequency > other.frequency;
    }
};

void generateCodes(std::vector<CodeInfo>& codes, int start, int end, const std::string& prefix);

bool writeDictionary(const std::string& filename, const std::map<unsigned char, std::string>& encodingMap);

bool readDictionary(const std::string& filename, std::map<std::string, unsigned char>& decodingMap);

#endif