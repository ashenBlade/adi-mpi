#include <iostream>
#include <omp.h>
#include <sstream>
#include <unistd.h>
#include <memory>
#include <vector>
#include "vectors_summarizing.h"
#include "matrix.h"

void vectorsMultiplyTimeCheck(size_t vectorsCount, size_t vectorSize);

void matrixMultiplyTimeCheck(size_t matrixSize);

void runInteractiveTest(int argc, char *const *argv);

static std::vector<std::vector<int>>* init_random_vectors(size_t vectors_count, size_t vector_size) {
    auto vectors = new std::vector<std::vector<int>>(vectors_count);
    omp_set_nested(true);
    for (int i = 0; i < vectors_count; ++i) {
        auto vector = std::vector<int>();
        for (int j = 0; j < vector_size; ++j) {
            vector.push_back(rand());
        }
        vectors->push_back(vector);
    }
    return vectors;
}

void printMatrix(const Matrix& matrix);

int main(int argc, char** argv) {
    omp_set_dynamic(true);
    omp_set_nested(true);
    runInteractiveTest(argc, argv);
}

void printMatrix(const Matrix& matrix) {
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix.size(); ++j) {
            std::cout << matrix[i][j] << ' ';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void runInteractiveTest(int argc, char *const *argv) {
    std::string runMode;

    // [0] is name of program
    if (argc > 1) {
        runMode = argv[1];
    } else {
        std::cout << "Enter run mode: 'matrix' or 'vectors'\n";
        std::cin >> runMode;
    }
    try {
        if (runMode == "matrix") {
            size_t matrixSize = 0;
            if (argc == 2) {
                matrixSize = std::stoll(argv[2]);
            } else {
                std::cout << "Enter matrix size: ";
                std::cin >> matrixSize;
            }
            matrixMultiplyTimeCheck(matrixSize);
        } else if (runMode == "vectors") {
            size_t vectorsCount = 0,
                    vectorSize = 0;
            if (argc == 4) {
                vectorsCount = std::stoll(argv[2]);
                vectorSize = std::stoll(argv[3]);
            } else {
                std::cout << "Enter vectors count: ";
                std::cin >> vectorsCount;
                std::cout << "Enter vector size: ";
                std::cin >> vectorSize;
            }
            vectorsMultiplyTimeCheck(vectorsCount, vectorSize);
        } else {
            std::cout << "Invalid input run mode\n";
            exit(1);
        }
    } catch (const std::exception& e) {
        std::cout << e.what();
        exit(1);
    }
}

void matrixMultiplyTimeCheck(size_t matrixSize) {
//    auto first = Matrix::createRandomMatrix(matrixSize);
//    auto second = Matrix::createRandomMatrix(matrixSize);
//
//    auto firstData = first->getData();
//    auto secondData = second->getData();
//    auto temp = Matrix(matrixSize);
//    auto temp2 = Matrix(matrixSize);
//
//#pragma omp parallel for collapse(2)
//    for (int i = 0; i < matrixSize; ++i) {
//        for (int j = 0; j < matrixSize; ++j) {
//            int result;
//#pragma omp parallel for reduction()
//            for (int k = 0; k < matrixSize; ++k) {
//
//            }
//        }
//    }
//
//    delete second;
//    delete first;

    if (matrixSize < 1) {
        throw std::runtime_error("Matrix size must be positive. Given: " + std::to_string(matrixSize));
    }
    auto left = std::unique_ptr<Matrix>(Matrix::createRandomMatrix(matrixSize));
    auto right = std::unique_ptr<Matrix>(Matrix::createRandomMatrix(matrixSize));

    auto start = omp_get_wtime();
    left->multiplyParallel(*right);
    auto end = omp_get_wtime();
    std::cout << "Execution time for parallel: " << end - start << std::endl;

    start = omp_get_wtime();
    left->multiplySingle(*right);
    end = omp_get_wtime();
    std::cout << "Execution time for single: " << end - start << std::endl;

}

void vectorsMultiplyTimeCheck(size_t vectorsCount, size_t vectorSize) {
    srand(time(nullptr));
    if (vectorsCount < 1) {
        throw std::runtime_error("Vectors count must be positive. Given: " + std::to_string(vectorsCount));
    }
    if (vectorSize < 1) {
        throw std::runtime_error("Vector size must be positive. Given: " + std::to_string(vectorsCount));
    }
    auto vectors = std::unique_ptr<std::vector<std::vector<int>>>(init_random_vectors(vectorsCount, vectorSize));

    auto start = omp_get_wtime();
    sum_vectors_parallel(*vectors);
    auto end = omp_get_wtime();
    std::cout << "Execution time for parallel: " << (end - start) << std::endl;

    start = omp_get_wtime();
    sum_vectors_single(*vectors);
    end = omp_get_wtime();
    std::cout << "Execution time for single: " << end - start << std::endl;
}

#pragma region DEPRECATED
void rewritten_main(int argc, char** argv) {
#define BUFFER_SIZE 10000
    std::cout << "Processors count: " << omp_get_num_procs() << "\n";
    int a[BUFFER_SIZE],
            b[BUFFER_SIZE],
            c[BUFFER_SIZE],
            d[BUFFER_SIZE];
#pragma omp parallel for shared(a, b, c, d, std::cout) default(none)
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        a[i] = i;
        b[i] = i;
        d[i] = 2 * i;
        std::stringstream ss;
        ss << "Current thread number:\t" << omp_get_thread_num() << "\n";
        ss << "Total threads number:\t" << omp_get_num_threads() << "\n";
        ss << "-----------------------------\n";
        std::cout << ss.str();
    }
#pragma omp parallel for shared(a, b, c, d) default(none)
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        c[i] = a[i] + b[i] + d[i];
    }

    std::cout << "a[" << BUFFER_SIZE / 2 << "] = " << a[BUFFER_SIZE / 2] << std::endl;
}
#pragma endregion