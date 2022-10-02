//
// Created by ashblade on 07.09.22.
//

#ifndef C_MATRIX_H
#define C_MATRIX_H



#include "unistd.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

class MatrixColumn {
private:
    int* _column;
    size_t _size;
public:
    MatrixColumn(int* column, size_t size);
    MatrixColumn(MatrixColumn&&) = delete;
    MatrixColumn(MatrixColumn&) = delete;
    ~MatrixColumn() = default;

    int operator[](size_t col) const;
    int& operator[](size_t col);

    size_t size() const;
};


class Matrix {
public:
    explicit Matrix(size_t size);
    Matrix(Matrix&&) = delete;
    Matrix(Matrix&) = delete;
    ~Matrix();

    size_t size() const;
    MatrixColumn operator[](int row) const;

    double multiplySequential(const Matrix& matrix) const;
    double multiplyStandard(const Matrix& matrix, int threadsCount) const;
    double multiplyBand(const Matrix& matrix, int threadsCount) const;
    double multiplyBlock(const Matrix &matrix, int threadsCount) const;

    static Matrix* createRandomMatrix(size_t size);

private:
    int** _data;
    size_t _size;

    Matrix(int** data, size_t size);
};

#endif //C_MATRIX_H
