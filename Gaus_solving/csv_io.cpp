
#include "csv_io.h"
#include <fstream>
#include <sstream>

bool readSystemCSV(const std::string& filename, Eigen::MatrixXd& A, Eigen::VectorXd& b) {
    std::ifstream file(filename);
    if (!file.is_open()) return false;
    std::vector<std::vector<double>> data;
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        std::vector<double> row;
        while (std::getline(ss, cell, ',')) {
            row.push_back(std::stod(cell));
        }
        data.push_back(row);
    }
    if (data.empty()) return false;
    size_t rows = data.size();
    size_t cols = data[0].size();
    A.resize(rows, cols - 1);
    b.resize(rows);
    for (size_t i = 0; i < rows; ++i) {
        for (size_t j = 0; j < cols - 1; ++j) {
            A(i, j) = data[i][j];
        }
        b(i) = data[i][cols - 1];
    }
    return true;
}

void writeSolutionCSV(const std::string& filename, const Eigen::VectorXd& x) {
    std::ofstream file(filename);
    for (int i = 0; i < x.size(); ++i) {
        file << x[i] << "\n";
    }
} 