#include <iostream>
#include <algorithm>
#include <vector>
#include "cmath"

#define PARTITIONS_COUNT (10)
#define X_MIN (0)
#define X_MAX (3.1415 / 2)
#define X_STEP ((X_MAX - X_MIN) / PARTITIONS_COUNT)

std::vector<double>* solveThomas() {
    const int N = PARTITIONS_COUNT;
    const double xStep = X_STEP;

    double A[N],
            B[N],
            C[N],
            f[N],
            F[N];

    auto Y = new std::vector<double>(N);
    auto& y = *Y;

    double x[N],
            D[N]; // В будущем будет значениями лямбда в точке (x, y)

    for (int i = 0; i < N; ++i) {
        x[i] = i * xStep;
        D[i] = 1;
        F[i] = -std::sin(x[i]);
        f[i] = -F[i] * xStep * xStep;
    }

    for (int i = 0; i < N; ++i) {
        A[i] = D[i];
        B[i] = D[i + 1];
        C[i] = D[i] + D[i + 1];
    }

    A[0] = B[N - 1] = 0;

    double alpha[N],
            beta[N];
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

    return Y;
}

int main(int argc, char** argv) {
    auto y = solveThomas();

    for (int i = 0; i < y->size(); ++i) {
        std::cout << "y[" << i << "] = " << y->operator[](i) << std::endl;
    }

    delete y;
    return 0;
}
