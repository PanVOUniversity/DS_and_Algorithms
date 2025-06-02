#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <fstream>
#include <stdexcept>

#include <Eigen/Dense>
#include "csv_io.h" // Include the custom CSV I/O header

using EigenRowMajorMatrixXd = Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;

class GaussianLinearSolver {
public:
    // Constructor takes an augmented matrix (A|b)
    explicit GaussianLinearSolver(const EigenRowMajorMatrixXd& augmented_matrix)
        : m_matrix(augmented_matrix) {
        // Basic validation for the augmented matrix
        if (m_matrix.rows() == 0) {
            throw std::runtime_error("Matrix cannot be empty.");
        }
        // An augmented matrix for Ax=b should have one more column than rows
        if (m_matrix.cols() != m_matrix.rows() + 1) {
            throw std::runtime_error("Augmented matrix must have one more column than rows (for the constants column).");
        }
    }

    // Solves the linear system using Gaussian elimination with partial pivoting
    Eigen::VectorXd solve() {
        int n = m_matrix.rows(); // Number of equations (and variables)

        // Forward elimination with partial pivoting
        for (int i = 0; i < n; ++i) {
            // Find the row with the maximum absolute value in the current column (for pivoting)
            int max_row_idx = i;
            for (int k = i + 1; k < n; ++k) {
                if (std::abs(m_matrix(k, i)) > std::abs(m_matrix(max_row_idx, i))) {
                    max_row_idx = k;
                }
            }

            // Swap the current row with the row found (if different)
            if (max_row_idx != i) {
                m_matrix.row(i).swap(m_matrix.row(max_row_idx));
            }

            // Check for a near-zero diagonal element after pivoting
            // This indicates a singular or ill-conditioned matrix
            if (std::abs(m_matrix(i, i)) < 1e-12) { // Using a small epsilon for floating-point comparison
                throw std::runtime_error("Error in matrix: zero or near-zero diagonal element. The system may be singular or have infinitely many solutions.");
            }

            // Eliminate elements below the current pivot
            for (int k = i + 1; k < n; ++k) {
                double factor = m_matrix(k, i) / m_matrix(i, i);
                // Apply the elimination factor to all elements in the row
                for (int j = i; j < n + 1; ++j) {
                    m_matrix(k, j) -= m_matrix(i, j) * factor;
                }
                // Explicitly set to zero to avoid floating point inaccuracies
                m_matrix(k,i) = 0.0;
            }
        }

        // Back substitution to find the solution vector
        Eigen::VectorXd solution(n);
        for (int i = n - 1; i >= 0; --i) {
            double sum = m_matrix(i, n); // The constant term (from the 'b' part of A|b)
            // Subtract the terms involving already solved variables
            for (int j = i + 1; j < n; ++j) {
                sum -= m_matrix(i, j) * solution(j);
            }
            // Check for near-zero diagonal element during back substitution
            if (std::abs(m_matrix(i, i)) < 1e-12) {
                 throw std::runtime_error("Error during back substitution: zero or near-zero diagonal element.");
            }
            // Calculate the current variable's solution
            solution(i) = sum / m_matrix(i, i);
        }
        return solution;
    }

private:
    EigenRowMajorMatrixXd m_matrix; // The augmented matrix (A|b)
};


// This block is compiled only if TESTING is not defined,
// allowing the main function to be excluded when building tests.
#ifndef TESTING
int main(int argc, char* argv[]) {
    // Check for correct number of command-line arguments
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input.csv> <output.csv>\n";
        return 1;
    }

    std::string input_filename = argv[1];
    std::string output_filename = argv[2];

    try {
        std::cout << "Reading data from file: " << input_filename << std::endl;

        Eigen::MatrixXd A; // Matrix A
        Eigen::VectorXd b; // Vector b

        // Read the system Ax=b from the CSV file using the custom utility
        if (!readSystemCSV(input_filename, A, b)) {
            std::cerr << "Error: Could not read system from CSV file '" << input_filename << "'." << std::endl;
            return 1;
        }

        // Construct the augmented matrix (A|b) from A and b
        // The augmented matrix has A on the left and b as the last column
        EigenRowMajorMatrixXd augmented_matrix(A.rows(), A.cols() + 1);
        augmented_matrix.leftCols(A.cols()) = A;
        augmented_matrix.rightCols(1) = b;

        std::cout << "Augmented matrix A|b:" << std::endl << augmented_matrix << std::endl << std::endl;

        // Create a solver instance with the augmented matrix
        GaussianLinearSolver solver(augmented_matrix);
        // Solve the system
        Eigen::VectorXd solution = solver.solve();
        std::cout << "Solution x:" << std::endl << solution << std::endl << std::endl;

        // Write the solution vector to a CSV file using the custom utility
        writeSolutionCSV(output_filename, solution);
        std::cout << "Solution written to file: " << output_filename << std::endl;

    } catch (const std::exception& e) {
        // Catch and print any exceptions that occur during execution
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return 1; // Indicate an error
    }

    return 0; // Indicate successful execution
}
#endif
