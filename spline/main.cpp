#include <cstdio>
#include <iostream>
#include "mpi.h"


#define INITIAL_MATRIX_VALUE 300
#define TIME_DELTA 0.0001


using matrix_t = double**;

matrix_t createInitialMatrix(int rows, int columns) {
    auto matrix = new double * [rows];
    for (int row = 0; row < rows; ++row) {
        auto column = new double[columns];
        for (int i = 0; i < columns; ++i) {
            column[i] = INITIAL_MATRIX_VALUE;
        }
        matrix[row] = column;
    }
    return matrix;
}

double getLambda(double x, double y) {
    return 0.25 <= x && x <= 0.65 &&
           0.1 <= y && y <= 0.25
           ? 0.01
           : 0.0001;
}



int main(int argc, char** argv) {
    double xMin = 0, xMax = 1;
    const int n = 10;
    double step = (xMax - xMin) / n;
    const int MAX_SIZE = n + 1;
    double A[MAX_SIZE],
            B[MAX_SIZE],
            C[MAX_SIZE],
            D[MAX_SIZE],
            f[MAX_SIZE],
            F[MAX_SIZE],
            x[MAX_SIZE];

    for (int i = 0; i < MAX_SIZE; ++i) {
        x[i] = xMin + step * i;
        D[i] = x[i];
        F[i] = 4 * x[i];
    }

    for (int i = 0; i < MAX_SIZE; ++i) {
        A[i] = D[i];
        B[i] = D[i + 1];
        C[i] = D[i] + D[i + 1];
        f[i] = -F[i] * (step * step);
    }
    B[MAX_SIZE - 1] = A[0] = 0;

    double alpha[MAX_SIZE],
            beta[MAX_SIZE];

    alpha[1] = 0; // хи 1
    beta[1] = 1; // мю 1

    for (int i = 1; i < MAX_SIZE - 1; ++i) {
        alpha[i + 1] =  B[i] / (C[i] - alpha[i] * A[i]);
        beta[i + 1] = (A[i] * beta[i] + F[i]) / (C[i] - alpha[i]  * A[i]);
    }

    double y[MAX_SIZE];
    y[MAX_SIZE - 1] = 1;

    for (int i = MAX_SIZE - 2; i >= 0; i--) {
        y[i] = alpha[i + 1] * y[i + 1] + beta[i + 1];
        std::cout << "y[" << i << "] = " << y[i] << "\n";
    }
    return 0;
}
