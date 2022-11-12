#include <cstdio>
#include <iostream>
#include "mpi.h"
#include <algorithm>
#include "cmath"

int main(int argc, char** argv) {
    const int n = 10;
    const int N = n + 1;
    const double xMin = 0,
                xMax = 3.1415 / 2;
    const double xStep = (xMax - xMin) / n;

    double A[N],
            B[N],
            C[N],
            f[N],
            F[N],
            y[N];

    double x[N], d[N];
    for (int i = 0; i < N; ++i) {
        x[i] = i * xStep;
        d[i] = 1;
        F[i] = -std::sin(x[i]);
        f[i] = -F[i] * xStep * xStep;
    }

    for (int i = 0; i < N; ++i) {
        A[i] = d[i];
        B[i] = d[i + 1];
        C[i] = d[i] + d[i + 1];
    }

    A[0] = B[N - 1] = 0;

    double alpha[N],
            beta[N];

//    alpha[0] = -B[0] / C[0];
//    beta[0] = F[0] / C[0];
    alpha[0] = 0;
    for (int i = 0; i < N - 1; ++i) {
        alpha[i + 1] =
                B[i]
                /
                (C[i] - A[i] * alpha[i]);
        beta[i + 1] =
                (f[i] + A[i] * beta[i])
                /
                (C[i] - A[i] * alpha[i]);
    }

    y[0] = 0;
    y[N - 1] = 1;

    for (int i = N - 2; i >= 0; --i) {
        y[i] = alpha[i + 1] * y[i + 1] + beta[i + 1];
    }

    for (int i = 0; i < N; ++i) {
        std::cout << "y[" << i << "] = " << y[i] << '\n';
    }
    return 0;
}
