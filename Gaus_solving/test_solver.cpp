#include <gtest/gtest.h>
#include <Eigen/Dense>
#include "csv_io.h"
#include <fstream>
#include <cstdio>

TEST(GaussianSolverTest, KnownSystem) {
    Eigen::MatrixXd A(2, 2);
    A << 2, 1,
         1, 3;
    Eigen::VectorXd b(2);
    b << 8, 13;
    Eigen::VectorXd x = A.colPivHouseholderQr().solve(b);
    Eigen::Vector2d expected(2.2, 3.6);
    EXPECT_NEAR((x - expected).norm(), 0.0, 1e-8);
}

TEST(GaussianSolverTest, CSVIO) {
    // Prepare a small system
    Eigen::MatrixXd A(2, 2);
    A << 1, 2,
         3, 4;
    Eigen::VectorXd b(2);
    b << 5, 6;
    // Write to CSV
    std::string filename = "test_system.csv";
    std::ofstream file(filename);
    for (int i = 0; i < 2; ++i) {
        file << A(i, 0) << "," << A(i, 1) << "," << b(i) << "\n";
    }
    file.close();
    // Read back
    Eigen::MatrixXd A2;
    Eigen::VectorXd b2;
    ASSERT_TRUE(readSystemCSV(filename, A2, b2));
    EXPECT_EQ(A2.rows(), 2);
    EXPECT_EQ(A2.cols(), 2);
    EXPECT_EQ(b2.size(), 2);
    EXPECT_NEAR((A - A2).norm(), 0.0, 1e-12);
    EXPECT_NEAR((b - b2).norm(), 0.0, 1e-12);
    std::remove(filename.c_str());
}

TEST(GaussianSolverTest, SixBySixSystem) {
    Eigen::MatrixXd A = Eigen::MatrixXd::Identity(6, 6);
    Eigen::VectorXd x_true(6);
    x_true << 1, 2, 3, 4, 5, 6;
    Eigen::VectorXd b = A * x_true;
    Eigen::VectorXd x = A.colPivHouseholderQr().solve(b);
    EXPECT_NEAR((x - x_true).norm(), 0.0, 1e-8);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 