#include <iostream>
#include <algorithm>
#include <vector>


double getLambda(double x, double y) {
    return 0.25 <= x && x <= 0.65 &&
    0.1 <= y && y <= 0.25 ? 0.01 : 0.0001;
}

std::vector<double>* restoreT(const std::vector<double>& T,
                              const std::vector<double>& lambda,
                              const double step,
                              const double timeStep) {
    if (lambda.size() != T.size()) {
        throw std::runtime_error("Размеры lambda и T не равны в restoreT");
    }

    auto& result = *new std::vector<double>();
    
    // крайние варианты

    double firstTemperature = T[0] / timeStep + (((lambda[0] + lambda[1]) / 2) * (T[1] - T[0])) / (2 * step * step);
    result.push_back(firstTemperature);
    for (int i = 1; i < T.size() - 1; ++i) {
        double currentTemperature = T[i] / timeStep + (((lambda[i] + lambda[i + 1]) / 2) * (T[i + 1] - T[i]) -
                           ((lambda[i] + lambda[i - 1]) / 2) * (T[i] - T[i - 1])) / (2 * step * step);
        result.push_back(currentTemperature);
    }
    auto last = T.size() - 1;

    double lastTemperature = T[last] / timeStep +
                             (((lambda[last] - lambda[last - 1]) / 2) * (T[last] - T[last - 1])) / (2 * step * step);
    result.push_back(lastTemperature);
    
    return &result;
}

double avg(double left, double right) {
    return (left + right) / 2;
}

/// lambda должна быть на 2 больше, чем T, чтобы корректно проинициализировать T
/// т.к. там используются средние значения
/// len(T) = length
/// len(lambda) = length
void solveThomas(const double* T,
                    const double* lambda,
                    const int length,
                    const double min,
                    const double max,
                    const double step,
                    const double timeStep,
                    double* y) {
    const auto coefficient = 1.0 / (2 * step * step);

    double A[length], B[length], C[length];
    for (int i = 0; i < length; ++i) {
        A[i] = - avg(lambda[i + 1], lambda[i]) / 2 * coefficient;
        B[i] = - avg(lambda[i + 1], lambda[i + 2]) / 2 * coefficient;
        C[i] = 1 / timeStep - A[i] - B[i];
    }

    double alpha[length], beta[length];
    alpha[0] = alpha[length - 1] = 0;
    beta[0] = min;
    beta[length - 1] = max;

    for (int i = 0; i < length - 1; ++i) {
        alpha[i + 1] = B[i] / (C[i] - A[i] * alpha[i]);
        beta[i + 1] = (T[i] + A[i] * beta[i]) / (C[i] - A[i] * alpha[i]);
    }

    // TODO: найти чему равно начальное (конечное) значение
    y[length - 1] = max;
    y[0] = min;
    for (int i = length - 2; i >= 0; i--) {
        y[i] = alpha[i] * y[i + 1] + beta[i];
    }
}


/// Восстановить значения по найденным Y
/// len(y) = size
/// len(lambda) = size + 2 (для промежуточных значений)
/// len(T) = size
void restoreValues(const double* y,
                   const double* lambda,
                   const int size,
                   const double step,
                   const double timeStep,
                   double* T) {
    for (int i = 1; i < size - 1; ++i) {
        double lambdaPlusHalf = avg(lambda[i + 1], lambda[i + 2]) / 2;
        double lambdaMinusHalf = avg(lambda[i + 1], lambda[i]) / 2;
        const auto value = y[i] / timeStep + (lambdaPlusHalf * (y[i + 1] - y[i]) - lambdaMinusHalf * (y[i] - y[i - 1])) / (step * step);
        T[i] = value;
    }
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

void logMatrix(double** matrix, int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << "\n";
    }
}


int main(int argc, char** argv) {
    const int size = 10;
    const double timeStep = 0.001;
    const double tLeft = 600;
    const double tRight = 1200;
    const double xMin = 0;
    const double xMax = 1;
    const double yMin = 0;
    const double yMax = 0.5;
    const double xStep = (xMax - xMin) / size;
    const double yStep = (yMax - yMin) / size;

    // y - исходная температура
    double temperatureByX[size][size];
    double lambdaByX[size][size];
    // F
    double TByX[size][size];
    for (int i = 0; i < size; ++i) {
        auto x = i * xStep;
        for (int j = 0; j < size; ++j) {
            auto y = j * yStep;
            lambdaByX[i][j] = getLambda(x, y);
            temperatureByX[i][j] = 300;
            TByX[i][j] = 0;
        }
    }

    for (int i = 0; i < size; ++i) {
        restoreValues(temperatureByX[i], lambdaByX[i], size, xStep, timeStep, TByX[i]);
    }
    const int iterations = 10;

    // Меняю строки и столбцы
    double temperatureByY[size][size];
    double TByY[size][size];
    double lambdaByY[size][size];

    for (int y = 0; y < size; ++y) {
        for (int x = 0; x < size; ++x) {
            temperatureByY[y][x] = temperatureByX[x][y];
            TByY[y][x] = TByX[x][y];
            lambdaByY[y][x] = lambdaByX[x][y];
        }
    }

    for (int t = 0; t < iterations; ++t) {
        // Вычисляю yn+1/2
        for (int x = 0; x < size; ++x) {
            solveThomas(TByX[x], lambdaByX[x], size, tLeft, tRight, xStep, timeStep, temperatureByY[x]);
        }

        // Вычисляю Fn+1/2
        for (int x = 0; x < size; ++x) {
            restoreValues(temperatureByX[x], lambdaByX[x], size, xStep, timeStep, TByX[x]);
        }

        for (int y = 0; y < size; ++y) {
            for (int x = 0; x < size; ++x) {
                temperatureByY[y][x] = temperatureByX[x][y];
                TByY[y][x] = TByX[x][y];
                lambdaByY[y][x] = lambdaByX[x][y];
            }
        }

        // Вычисляю yn+1
        for (int y = 0; y < size; ++y) {
            double x = xMin + y * xStep;
            solveThomas(TByY[y], lambdaByY[y], size, 600 * (1 + x), 600 * (1 + x * x * x), yStep, timeStep, temperatureByY[y]);
        }

        // Вычисляю Fn+1
        for (int y = 0; y < size; ++y) {
            restoreValues(temperatureByY[y], lambdaByY[y], size, yStep, timeStep, TByY[y]);
        }


        for (int x = 0; x < size; ++x) {
            for (int y = 0; y < size; ++y) {
                temperatureByX[x][y] = temperatureByY[y][x];
                TByX[x][y] = TByY[y][x];
                lambdaByX[x][y] = lambdaByY[y][x];
            }
        }

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                std::cout << temperatureByX[i][j] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n\n";
    }

}
