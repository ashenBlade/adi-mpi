#include <cstdio>
#include <iostream>
#include "mpi.h"

#define BETA_0 0
#define BETA_N 1



int main(int argc, char** argv) {
    double xMin = 0, xMax = 1;
    const int n = 10;
    const int MAX_SIZE = n + 1;
    double step = (xMax - xMin) / n;
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

    alpha[0] = alpha[MAX_SIZE - 1] = 0;
    beta[0] = BETA_0;
    for (int i = 1; i < MAX_SIZE - 1; ++i) {
        alpha[i] =  -B[i] / (C[i] + alpha[i - 1] * A[i]);
        beta[i] = (A[i] * beta[i - 1] + F[i]) / (C[i] - alpha[i - 1]  * A[i]);
    }
    beta[MAX_SIZE - 1] = BETA_N;

    double y[MAX_SIZE];
    y[MAX_SIZE - 1] = 1;

    for (int i = MAX_SIZE - 2; i >= 0; --i) {
        y[i] = alpha[i + 1] * y[i + 1] + beta[i + 1];
        std::cout << "y[" << i << "] = " << y[i] << "\n";
    }
    return 0;
}
