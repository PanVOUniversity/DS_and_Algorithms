#include "ShannonCoding.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <gtest/gtest.h> // Include Google Test header

// Function declarations (from Encoder.cpp and Decoder.cpp)
bool encodeFile(const std::string& inputFile, const std::string& compressedFile, const std::string& dictionaryFile);
bool decodeFile(const std::string& compressedFile, const std::string& dictionaryFile, const std::string& decodedFile);

// Helper function to compare two files, now integrated into tests
bool compareFiles(const std::string& file1, const std::string& file2) {
    std::ifstream f1(file1, std::ios::binary);
    std::ifstream f2(file2, std::ios::binary);

    if (!f1.is_open() || !f2.is_open()) {
        std::cerr << "Oopsie: Couldn't open one of the files for comparison." << std::endl;
        return false;
    }

    f1.seekg(0, std::ios::end);
    f2.seekg(0, std::ios::end);
    if (f1.tellg() != f2.tellg()) {
        std::cerr << "File sizes are different." << std::endl;
        return false;
    }

    f1.seekg(0, std::ios::beg);
    f2.seekg(0, std::ios::beg);

    char ch1, ch2;
    while (f1.get(ch1) && f2.get(ch2)) {
        if (ch1 != ch2) {
            std::cerr << "File contents are different." << std::endl;
            return false;
        }
    }

    return true;
}

// Define a test fixture for file operations
class ShannonCodecTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Set up common file names for tests
        testFileName = "gtest_temp_file.txt";
        compressedFile = testFileName + ".shannon";
        dictionaryFile = testFileName + ".dict";
        decodedFile = testFileName + ".decoded";
    }

    void TearDown() override {
        // Clean up files after each test
        remove(testFileName.c_str());
        remove(compressedFile.c_str());
        remove(dictionaryFile.c_str());
        remove(decodedFile.c_str());
    }

    void CreateTestFile(const std::string& content) {
        std::ofstream testFile(testFileName, std::ios::binary);
        ASSERT_TRUE(testFile.is_open()) << "Oopsie: Couldn't create test file: " << testFileName;
        testFile << content;
        testFile.close();
    }

    std::string testFileName;
    std::string compressedFile;
    std::string dictionaryFile;
    std::string decodedFile;
};

// Test case for a basic text file
TEST_F(ShannonCodecTest, BasicTextFile) {
    CreateTestFile("Hello, Google Test!");
    ASSERT_TRUE(encodeFile(testFileName, compressedFile, dictionaryFile));
    ASSERT_TRUE(decodeFile(compressedFile, dictionaryFile, decodedFile));
    ASSERT_TRUE(compareFiles(testFileName, decodedFile));
}

// Test case for a file with various characters and repetitions
TEST_F(ShannonCodecTest, ComplexTextFile) {
    CreateTestFile("This is a test file to check the Shannon encoder and decoder. "
                   "It contains various characters and repeating sequences. "
                   "The quick brown fox jumps over the lazy dog. 1234567890!@#$%^&*()");
    ASSERT_TRUE(encodeFile(testFileName, compressedFile, dictionaryFile));
    ASSERT_TRUE(decodeFile(compressedFile, dictionaryFile, decodedFile));
    ASSERT_TRUE(compareFiles(testFileName, decodedFile));
}

// Test case for a binary file (e.g., with null bytes)
TEST_F(ShannonCodecTest, BinaryFile) {
    CreateTestFile(std::string(256, 'A') + std::string(128, 'B') + std::string(64, '\0'));
    ASSERT_TRUE(encodeFile(testFileName, compressedFile, dictionaryFile));
    ASSERT_TRUE(decodeFile(compressedFile, dictionaryFile, decodedFile));
    ASSERT_TRUE(compareFiles(testFileName, decodedFile));
}

// Test case for an empty file
TEST_F(ShannonCodecTest, EmptyFile) {
    CreateTestFile("");
    ASSERT_TRUE(encodeFile(testFileName, compressedFile, dictionaryFile));
    ASSERT_TRUE(decodeFile(compressedFile, dictionaryFile, decodedFile));
    ASSERT_TRUE(compareFiles(testFileName, decodedFile));
}

// Test case for a file with a single character
TEST_F(ShannonCodecTest, SingleCharacterFile) {
    CreateTestFile("A");
    ASSERT_TRUE(encodeFile(testFileName, compressedFile, dictionaryFile));
    ASSERT_TRUE(decodeFile(compressedFile, dictionaryFile, decodedFile));
    ASSERT_TRUE(compareFiles(testFileName, decodedFile));
}

// Test case for a file with many repeated characters
TEST_F(ShannonCodecTest, RepeatedCharacterFile) {
    CreateTestFile(std::string(1000, 'X'));
    ASSERT_TRUE(encodeFile(testFileName, compressedFile, dictionaryFile));
    ASSERT_TRUE(decodeFile(compressedFile, dictionaryFile, decodedFile));
    ASSERT_TRUE(compareFiles(testFileName, decodedFile));
}

int main(int argc, char **argv) {
    setlocale(LC_ALL, "en_US.UTF-8"); // Changed to English locale
    std::cout << "--- Shannon Encoder and Decoder Google Tests ---" << std::endl;
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

