#include <iostream>
#include <algorithm>
#include <vector>

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

#define TAU 0.1

double getLambda(double x, double y) {
    return 0.25 <= x && x <= 0.65 &&
    0.1 <= y && y <= 0.25 ? 0.01 : 0.0001;
}

std::vector<double>* restoreTemperature(const std::vector<double>& y, 
                                        const std::vector<double>& lambda, 
                                        const double step,
                                        const double timeStep) {
    if (lambda.size() != y.size()) {
        throw std::runtime_error("Размеры lambda и y не равны в restoreTemperature");
    }

    auto& result = *new std::vector<double>();
    
    // крайние варианты

    double firstTemperature = y[0] / timeStep + (((lambda[0] + lambda[1]) / 2) * (y[1] - y[0])) / (2 * step * step);
    result.push_back(firstTemperature);
    for (int i = 1; i < y.size() - 1; ++i) {
        double currentTemperature = y[i] / timeStep + (((lambda[i] + lambda[i + 1]) / 2) * (y[i + 1] - y[i]) -
                           ((lambda[i] + lambda[i - 1]) / 2) * (y[i] - y[i - 1])) / (2 * step * step);
        result.push_back(currentTemperature);
    }
    auto last = y.size() - 1;

    double lastTemperature = y[last] / timeStep +
                             (((lambda[last] - lambda[last - 1]) / 2) * (y[last] - y[last - 1])) / (2 * step * step);
    result.push_back(lastTemperature);
    
    return &result;
}

/// Решаем прогонку в 1 измерении
std::vector<double>* solveThomas(const int partitionsCount,
                                 const double yMin,
                                 const double yMax,
                                 const std::vector<double>& lambda,
                                 const std::vector<double>& currentTemperature,
                                 const double step) {
    const int N = partitionsCount;
//    const double step = (yMax - yMin) / partitionsCount;

    auto Y = new std::vector<double>(N);
    auto& y = *Y;
    auto fuck = 1.0 / 2 * step * step;
    double A[N], B[N], C[N];

    A[N - 1] = -((lambda[N - 1] + lambda[N - 2]) / 2) * fuck;
    B[0] = -((lambda[0] + lambda[1]) / 2) * fuck;

    for (int i = 0; i < N - 1; ++i) {
        if (i != 0 && i != N - 1) {
            A[i] = -((lambda[i] + lambda[i - 1]) / 2) * fuck;
            B[i] = -((lambda[i] + lambda[i + 1]) / 2) * fuck;
        }
        C[i] = (1 / TAU) - A[i] - B[i];
        //        A[i] = lambda[i];
//        B[i] = lambda[i + 1];
//        C[i] = lambda[i] + lambda[i + 1];
    }

    A[0] = B[N - 1] = 0;

    double alpha[N],
            beta[N];
    alpha[0] = 0;
    beta[N - 1] = 0;
//    beta[0] = 600;
    for (int i = 0; i < N - 1; ++i) {
        alpha[i + 1] = B[i] / (C[i] - A[i] * alpha[i]);
        beta[i + 1] = (currentTemperature[i] + A[i] * beta[i]) / (C[i] - A[i] * alpha[i]);
    }

    // Граничные условия
    y[0] = yMin;
    y[N - 1] = yMax;

    for (int i = N - 2; i >= 1; --i) {
        y[i] = alpha[i] * y[i + 1] + beta[i];
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
    double step = (X_MAX - X_MIN) / X_PARTITIONS_COUNT;
    for (int i = 0; i < X_PARTITIONS_COUNT; ++i) {
        lambda.push_back(getLambda(i, 0));
        f.push_back(300);
    }

    int xStart = 600;
    int xEnd = 1200;
    auto y = solveThomas(
            X_PARTITIONS_COUNT,
            xStart,
            xEnd,
            lambda,
            f,
            step);
    auto yy = restoreTemperature(*y, lambda, step, TAU);
    std::cout << std::fixed;
    yy->operator[](0) = xStart;
    yy->operator[](yy->size() - 1) = xEnd;

    for (int i = 0; i < yy->size(); ++i) {
        std::cout << i << " " << yy->operator[](i) << std::endl;
//        std::cout << std::endl;
    }

    delete y;
    delete yy;
    return 0;
}
