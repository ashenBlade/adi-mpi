#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "cmath"

#define DEFAULT_PARTITIONS_COUNT 10

#define X_PARTITIONS_COUNT (DEFAULT_PARTITIONS_COUNT)
#define X_MIN (0.0)
#define X_MAX (1.0)
#define X_STEP ((X_MAX - X_MIN) / X_PARTITIONS_COUNT)

#define Y_PARTITIONS_COUNT (DEFAULT_PARTITIONS_COUNT)
#define Y_MIN (0.0)
#define Y_MAX (0.5)
#define Y_STEP ((Y_MAX - Y_MIN) / Y_PARTITIONS_COUNT)

#define T_START 0
#define T_END 10

double getLambda(double x, double y) {
    return 0.25 <= x && x <= 0.65 &&
    0.1 <= y && y <= 0.25 ? 0.01 : 0.0001;
}

/// Решаем прогонку в 1 измерении
std::vector<double>* solveThomas(const int partitionsCount,
                                 const double yMin,
                                 const double yMax,
                                 const std::vector<double>& lambda,
                                 const std::vector<double>& f) {
    const int N = partitionsCount;
//    const double step = (yMax - yMin) / partitionsCount;

    auto Y = new std::vector<double>(N);
    auto& y = *Y;

    double A[N], B[N], C[N];
    for (int i = 0; i < N; ++i) {
        A[i] = lambda[i];
        B[i] = lambda[i + 1];
        C[i] = lambda[i] + lambda[i + 1];
    }

    A[0] = B[N - 1] = 0;

    double alpha[N],
            beta[N];
    alpha[0] = 0;
    for (int i = 0; i < N - 1; ++i) {
        alpha[i + 1] = B[i] / (C[i] - A[i] * alpha[i]);
        beta[i + 1] = (f[i] + A[i] * beta[i]) / (C[i] - A[i] * alpha[i]);
    }

    // Граничные условия
    y[0] = yMin;
    y[N - 1] = yMax;

    for (int i = N - 2; i >= 0; --i) {
        y[i] = alpha[i + 1] * y[i + 1] + beta[i + 1];
    }

    return Y;
}

std::vector<std::vector<double>*>* initField(int rowsCount, int columnsCount) {
    auto rows = new std::vector<std::vector<double>*>();
    for (int i = 0; i < rowsCount; ++i) {
        auto column = new std::vector<double>();
        for (int j = 0; j < columnsCount; ++j) {
            column->push_back(300);
        }
        rows->push_back(column);
    }
    return rows;
}


int main(int argc, char** argv) {
    std::vector<double> lambda;
    std::vector<double> F;

    std::vector<double> f;
    for (int i = 0; i < X_PARTITIONS_COUNT; ++i) {
        lambda.push_back(getLambda(0, 0));
        f.push_back(300);
    }
    f[0] = 600;
    f[X_PARTITIONS_COUNT - 1] = 1200;

    auto y = solveThomas(X_PARTITIONS_COUNT, X_MIN, X_MAX, lambda, f);
    for (int i = 0; i < y->size(); ++i) {
        std::cout << y->operator[](i) << std::endl;
    }

    delete y;
    return 0;
}
