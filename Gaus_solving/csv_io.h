#pragma once
#include <Eigen/Dense>
#include <string>

// Reads a system Ax = b from a CSV file. The last column is b.
bool readSystemCSV(const std::string& filename, Eigen::MatrixXd& A, Eigen::VectorXd& b);
// Writes a solution vector x to a CSV file.
void writeSolutionCSV(const std::string& filename, const Eigen::VectorXd& x); 