//
// Created by ashblade on 07.09.22.
//

#include "matrix.h"
#include <stdexcept>
#include <functional>
#include <random>

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

static int **createEmptyMatrix(size_t size) {
    return createMatrixData(size, []() { return 0; });
}

static int** createRandomMatrixInner(size_t size) {
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



Matrix Matrix::multiplyParallel(const Matrix &matrix) const {
    auto& left = *this;
    auto& right = matrix;
    checkMatricesLength(left, right);

    size_t size = left.size();
    auto data = createEmptyMatrix(size);

#pragma omp parallel for shared(data, left, right, size) default(none)
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                data[i][j] += left[i][k] * right[k][j];
            }
        }
    }
    return {data, size};
}


Matrix Matrix::multiplySingle(const Matrix &matrix) const {
    auto& left = *this;
    auto& right = matrix;
    size_t size = matrix.size();
    checkMatricesLength(left, right);

    int **data = createEmptyMatrix(size);
    for (int k = 0; k < size; ++k) {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                data[i][j] += left[i][k] * right[k][j];
            }
        }
    }

    return {data, size};
}





Matrix::Matrix(int **data, size_t size) {
    this->_data = data;
    this->_size = size;
}



Matrix *Matrix::createRandomMatrix(size_t size) {
    return new Matrix(createRandomMatrixInner(size), size);
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





