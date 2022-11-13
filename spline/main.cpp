#include <iostream>
#include <algorithm>
#include <vector>
#include "cmath"

#define X_PARTITIONS_COUNT (10)
#define X_MIN (0)
#define X_MAX (1)

#define Y_PARTITIONS_COUNT (10)
#define Y_MIN (0)
#define Y_MAX (0.5)

double getLambda(double x, double y) {
    return 0.25 <= x && x <= 0.65 &&
    0.1 <= y && y <= 0.25 ? 0.01 : 0.0001;
}

std::vector<double>* solveThomas(int partitionsCount, double yMin, double yMax) {
    const int N = partitionsCount;
    const double step = (yMax - yMin) / partitionsCount;

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
        x[i] = i * step;
        D[i] = 1;
        F[i] = -std::sin(x[i]);
        f[i] = -F[i] * step * step;
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

    // Граничные условия
    y[0] = yMin;
    y[N - 1] = yMax;

    for (int i = N - 2; i >= 0; --i) {
        y[i] = alpha[i + 1] * y[i + 1] + beta[i + 1];
    }

    return Y;
}



int main(int argc, char** argv) {
    auto y = solveThomas(X_PARTITIONS_COUNT, X_MIN, X_MAX);

    for (int i = 0; i < y->size(); ++i) {
        std::cout << "y[" << i << "] = " << y->operator[](i) << std::endl;
    }

    delete y;
    return 0;
}
