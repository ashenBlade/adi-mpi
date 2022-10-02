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


double multiplyStandard(int threads, int size) {
    auto x = Matrix::createRandomMatrix(size);
    auto y = Matrix::createRandomMatrix(size);
    auto& left = *x;
    auto& right = *y;
    auto result = Matrix(size);
    auto start = omp_get_wtime();
#pragma omp parallel for shared(size, left, right, result) default(none) num_threads(threads)
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                result[i][j] += left[i][k] * right[k][j];
            }
        }
    }
    auto elapsed = omp_get_wtime() - start;
    delete x;
    delete y;
}


double multiplyPipe(int threads, int size) {
    auto x = Matrix::createRandomMatrix(size);
    auto y = Matrix::createRandomMatrix(size);
    auto& left = *x;
    auto& right = *y;
    auto result = Matrix(size);
    auto start = omp_get_wtime();
#pragma omp parallel for collapse(2) shared(size, left, right, result) default(none) num_threads(threads)
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j) {
            int sum = 0;
            for (int k = 0; k < size; ++k) {
                sum += left[i][k] * right[k][j];
            }
            result[i][j] = sum;
    }
    auto elapsed = omp_get_wtime() - start;
    delete x;
    delete y;
}

void multiplyBlockInner(int** left, int** right, int** result, int k, int i, int j, int size, int blockSize) {
    int start = blockSize * k;
    int end = std::min(start + blockSize, size);
    for (int ii = start; ii < end; ++ii) {
        for (int jj = start; jj < end; ++jj) {
            for (int kk = 0; kk < blockSize; ++kk) {
                result[i][j] += left[i][k] * right[k][j];
            }
        }
    }
}

double multiplyBlock(int threads, int size, int blockSize) {
    auto left = createRandomMatrixInner(size);
    auto right = createRandomMatrixInner(size);
    auto result = createEmptyMatrix(size);
    int blocksCount = size / blockSize;
    auto start = omp_get_wtime();

    for (int i = 0; i < blocksCount; ++i) {
        for (int j = 0; j < blocksCount; ++j) {
            for (int k = 0; k < blocksCount; ++k) {
                
            }
        }
    }

    auto elapsed = omp_get_wtime() - start;
    for (int i = 0; i < size; ++i) {
        delete[] left[i];
        delete[] right[i];
        delete[] result[i];
    }
    delete[] left;
    delete[] right;
    delete[] result;
    throw std::exception();
}


void measure11() {
    auto sizes = {1000, 1500, 2000, 2500, 3000};
    auto threadsCount = {2, 4, 8};
    std::vector<std::vector<Measurement>> measurements;
    for (const auto &size: sizes) {
        std::vector<Measurement> m;
        m.push_back(multiplySequential(size));
        for (const auto &threads: threadsCount) {
            m.push_back(multiplyStandard(threads, size));
            m.push_back(multiplyPipe(threads, size));
            m.push_back(multiplyBlock(threads, size));
        }
    }
}

