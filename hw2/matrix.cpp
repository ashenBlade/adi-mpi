//
// Created by ashblade on 07.09.22.
//

#include "memory"
#include "matrix.h"
#include <stdexcept>
#include <functional>
#include <random>
#include <omp.h>
#include <iostream>
#include "sstream"

static void destructMatrixData(int** data, size_t size) {
    for (int i = 0; i < size; ++i) {
        delete[] data[i];
    }
    delete[] data;
}

static int** createMatrixData(size_t size, const std::function<int()>& getValueFunc) {
    auto data = new int*[size];
    for (int i = 0; i < size; ++i) {
        auto row = new int[size];
        for (int j = 0; j < size; ++j) {
            row[j] = getValueFunc();
        }
        data[i] = row;
    }
    return data;
}

static void checkMatricesLength(const Matrix& left, const Matrix& right) {
    if (left.size() != right.size()) {
        throw std::invalid_argument("Matrices sizes not equal: " +
                                    std::to_string(right.size()) +
                                    " and " +
                                    std::to_string(left.size()));
    }
}

int **createEmptyMatrix(size_t size) {
    return createMatrixData(size, []() { return 0; });
}

int** createRandomMatrixInner(size_t size) {
    return createMatrixData(size, [=]() { return rand(); });
}

Matrix::Matrix(size_t size) {
    this->_size = size;
    this->_data = createEmptyMatrix(size);
}

size_t Matrix::size() const {
    return this->_size;
}

Matrix::~Matrix() {
    for (int i = 0; i < this->_size; ++i) {
        delete[] this->_data[i];
    }
    delete[] this->_data;
    this->_size = 0;
}

MatrixColumn Matrix::operator[](int row) const {
    return {this->_data[row], this->_size};
}


double Matrix::multiplySequential(const Matrix &matrix) const {
    auto& left = this->_data;
    auto& right = matrix._data;
    size_t size = matrix.size();
    checkMatricesLength(*this, matrix);
    int **data = createEmptyMatrix(size);
    auto start = omp_get_wtime();
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            int sum = 0;
            for (int k = 0; k < size; ++k) {
                sum += left[i][k] * right[k][j];
            }
            data[i][j] = sum;
        }
    }
    auto end = omp_get_wtime() - start;
    destructMatrixData(data, size);
    return end;
}

Matrix::Matrix(int **data, size_t size) {
    this->_data = data;
    this->_size = size;
}

Matrix *Matrix::createRandomMatrix(size_t size) {
    return new Matrix(createRandomMatrixInner(size), size);
}

double Matrix::multiplyStandard(const Matrix &matrix, int threadsCount) const {
    auto& left = this->_data;
    auto& right = matrix._data;
    checkMatricesLength(*this, matrix);

    size_t size = matrix.size();
    auto data = createEmptyMatrix(size);
    auto start = omp_get_wtime();
#pragma omp parallel for shared(data, left, right, size) default(none) num_threads(threadsCount)
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j) {
            int result = 0;

            for (int k = 0; k < size; ++k) {
                result += left[i][k] * right[k][j];
            }
            data[i][j] = result;
        }

    auto end = omp_get_wtime() - start;
    destructMatrixData(data, size);
    return end;
}


size_t getBlocksSideCount(size_t size, int blockSize) {
    auto side = size / blockSize;
    if (side * blockSize != size) {
        throw std::runtime_error("Matrix size can not be divided");
    }
    return side;
}

static void multiplyBlockInner(int** left, int** right, int** result, int i, int j, int size, int blockSize) {
    int chunksCount = size / blockSize;
    int iStart, iEnd, jStart, jEnd, kStart, kEnd;
    for (int chunk = 0; chunk < chunksCount; ++chunk) {
        iStart = jStart = kStart = chunk * blockSize;
        iEnd = jEnd = kEnd = iStart + blockSize;
        for (int ii = iStart; ii < iEnd; ++ii) {
            for (int jj = jStart; jj < jEnd; ++jj) {
                int sum = 0;
                for (int kk = kStart; kk < kEnd; ++kk) {
                    sum += left[ii][kk] * right[kk][jj];
                }
                result[ii][jj] += sum;
            }
        }
    }
}

double Matrix::multiplyBlock(const Matrix &matrix, int threadsCount) const {
    checkMatricesLength(*this, matrix);
    size_t size = matrix.size();
    const int blockSize = 100;
    auto blocksCount = getBlocksSideCount(size, blockSize);

    auto left = matrix._data;
    auto right = this->_data;
    auto result = createEmptyMatrix(size);

    auto start = omp_get_wtime();

#pragma omp parallel for collapse(2) shared(left, right, result) firstprivate(size, blockSize, blocksCount) default(none) num_threads(threadsCount)
    for (int i = 0; i < blocksCount; ++i)
        for (int j = 0; j < blocksCount; ++j) {
            multiplyBlockInner(left, right, result, i, j, size, blockSize);
        }

    auto elapsed = omp_get_wtime() - start;
    destructMatrixData(result, size);
    return elapsed;
}


double Matrix::multiplyBand(const Matrix &matrix, int threadsCount) const {
    auto& left = this->_data;
    auto& right = matrix._data;
    checkMatricesLength(*this, matrix);

    size_t size = matrix.size();
    auto data = createEmptyMatrix(size);
    auto start = omp_get_wtime();
#pragma omp parallel for collapse(2) shared(data, left, right, size) default(none) num_threads(threadsCount)
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j) {
            int result = 0;

            for (int k = 0; k < size; ++k) {
                result += left[i][k] * right[k][j];
            }

            data[i][j] = result;
        }

    auto end = omp_get_wtime() - start;
    destructMatrixData(data, size);
    return end;
}

MatrixColumn::MatrixColumn(int *column, size_t size) {
    this->_size = size;
    this->_column = column;
}

int MatrixColumn::operator[](size_t col) const {
    return this->_column[col];
}

int& MatrixColumn::operator[](size_t col) {
    return this->_column[col];
}

size_t MatrixColumn::size() const {
    return this->_size;
}





