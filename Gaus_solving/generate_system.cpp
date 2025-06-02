#include <fstream>
#include <random>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0] << " <size> <output.csv> <seed>\n";
        return 1;
    }
    int n = std::stoi(argv[1]);
    std::string filename = argv[2];
    unsigned seed = std::stoul(argv[3]);
    std::mt19937 gen(seed);
    std::uniform_real_distribution<> dis(-10, 10);
    std::ofstream file(filename);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            file << dis(gen) << ",";
        }
        file << dis(gen) << "\n"; // last column is b
    }
    return 0;
} 