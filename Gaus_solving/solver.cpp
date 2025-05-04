#include <Eigen/Dense>
#include <iostream>
#include "csv_io.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input.csv> <output.csv>\n";
        return 1;
    }
    Eigen::MatrixXd A;
    Eigen::VectorXd b;
    if (!readSystemCSV(argv[1], A, b)) {
        std::cerr << "Failed to read system from CSV\n";
        return 1;
    }
    Eigen::VectorXd x = A.colPivHouseholderQr().solve(b);
    writeSolutionCSV(argv[2], x);
    return 0;
}