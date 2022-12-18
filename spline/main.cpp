#include <iostream>
#include <mpi.h>

double getLambda(double x, double y) {
    return 0.25 <= x && x <= 0.65 &&
    0.1 <= y && y <= 0.25 ? 0.01 : 0.0001;
}

double avg(double left, double right) {
    return (left + right) / 2;
}

/// lambda должна быть на 2 больше, чем F, чтобы корректно проинициализировать F
/// т.к. там используются средние значения
/// len(F) = length
/// len(lambda) = length
void solveThomas(const double* F,
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

    for (int i = 0; i < length - 2; ++i) {
        alpha[i + 1] = - B[i + 1] / (C[i + 1] + A[i + 1] * alpha[i]);
        beta[i + 1] = (F[i + 1] - A[i + 1] * beta[i]) / (C[i + 1] + A[i + 1] * alpha[i]);
    }

    y[length - 1] = max;
    for (int i = length - 2; i >= 0; i--) {
        y[i] = alpha[i] * y[i + 1] + beta[i];
    }
}

void restoreValues(const double* y,
                   const double* lambda,
                   const int size,
                   const double step,
                   const double timeStep,
                   double* T) {
    for (int i = 1; i < size - 1; ++i) {
        double lambdaPlusHalf = avg(lambda[i], lambda[i + 1]);
        double lambdaMinusHalf = avg(lambda[i], lambda[i - 1]);
        const auto value = y[i] / timeStep + (lambdaPlusHalf * (y[i + 1] - y[i]) - lambdaMinusHalf * (y[i] - y[i - 1])) / (2 * step * step);
        T[i] = value;
    }
}

void logMatrix(double* matrix, const int size) {
    for (int i = 1; i < size - 1; ++i) {
        for (int j = 1; j < size - 1; ++j) {
            std::cout << matrix[i * size + j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << std::endl;
}

int main(int argc, char** argv) {
    const int size = 10;
    const int iterations = 1000;
    const int logStep = 100;
    const double timeStep = 0.1;
    const double TxLeft = 600;
    const double TxRight = 1200;
    const double xStart = 0;
    const double xEnd = 1;
    const double yStart = 0;
    const double yEnd = 0.5;
    const double xStep = (xEnd - xStart) / size;
    const double yStep = (yEnd - yStart) / size;

    int processesCount, rank;
    
    double lambdaByX[size][size];
    double lambdaByY[size][size];

    for (int i = 0; i < size; ++i) {
        const double xValue = xStart + i * xStep;
        for (int j = 0; j < size; ++j) {
            const double yValue = yStart + j * yStep;
            double lambda = getLambda(xValue, yValue);
            lambdaByX[i][j] = lambda;
            lambdaByY[j][i] = lambda;
        }
    }

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &processesCount);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (processesCount != size) {
        if (rank == 0) {
            std::cout << "Количество процессов должно быть " << size << std::endl;
        }
        MPI_Finalize();
        return 0;
    }

    MPI_Datatype matrixColumnsType, columnType;

    MPI_Type_vector(size, 1, size, MPI_DOUBLE, &matrixColumnsType);
    MPI_Type_commit(&matrixColumnsType);

    MPI_Type_create_resized(matrixColumnsType, 0, sizeof(double), &columnType);
    MPI_Type_commit(&columnType);

    double* myLambdaX = lambdaByX[rank];
    double* myLambdaY = lambdaByY[rank];

    double temperatureReceive[size];
    double fReceive[size];

    if (rank == 0) {
        double temperature[size * size];
        double F[size * size];

        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                temperature[i * size + j] = 300;
                F[i * size + j] = 0;
            }
        }

        // Восстанавливаем первое значение F (F0)
        for (int i = 0; i < size; ++i) {
            restoreValues((temperature + i * size), lambdaByX[i], size, xStep, timeStep, (F + i * size));
        }

        for (int t = 0; t < iterations; ++t) {
            // Вычисляю yn+1/2
            MPI_Scatter(F, size, MPI_DOUBLE,
                        fReceive, size, MPI_DOUBLE,
                        0, MPI_COMM_WORLD);

            solveThomas(fReceive, myLambdaX, size, TxLeft, TxRight, xStep, timeStep, temperatureReceive);
            restoreValues(temperatureReceive, myLambdaX, size, xStep, timeStep, fReceive);

            MPI_Gather(temperatureReceive, size, MPI_DOUBLE,
                       temperature, 1, columnType,
                       0, MPI_COMM_WORLD);
            MPI_Gather(fReceive, size, MPI_DOUBLE,
                       F, 1, columnType,
                       0, MPI_COMM_WORLD);

            MPI_Scatter(temperature, 1, columnType,
                        temperatureReceive, size, MPI_DOUBLE,
                        0, MPI_COMM_WORLD);
            MPI_Scatter(F, 1, columnType,
                        fReceive, size, MPI_DOUBLE,
                        0, MPI_COMM_WORLD);

            solveThomas(fReceive, myLambdaX, size, TxLeft, TxRight, xStep, timeStep, temperatureReceive);
            restoreValues(temperatureReceive, myLambdaX, size, xStep, timeStep, fReceive);

            // Аналогично принимаем только 1 элемент
            MPI_Gather(temperatureReceive, size, MPI_DOUBLE,
                       temperature, 1, columnType,
                       0, MPI_COMM_WORLD);
            MPI_Gather(fReceive, size, MPI_DOUBLE,
                       F, 1, columnType,
                       0, MPI_COMM_WORLD);


            if (t % logStep == 0) {
                logMatrix(temperature, size);
            }
        }
    } else {
        // Всего есть size столбцов/строк - остальные не нужны
        if (rank > size) {
            MPI_Finalize();
            return 0;
        }

        for (int t = 0; t < iterations; ++t) {

            /// Вычисляю yn+1/2 и Fn+1/2
            // Получаю значения fReceive по X
            MPI_Scatter(fReceive, size, MPI_DOUBLE,
                        fReceive, size, MPI_DOUBLE,
                        0, MPI_COMM_WORLD);

            // Само вычисление
            solveThomas(fReceive, myLambdaX, size, TxLeft, TxRight, xStep, timeStep, temperatureReceive);
            restoreValues(temperatureReceive, myLambdaX, size, xStep, timeStep, fReceive);

            // Возвращаю полученные данные
            MPI_Gather(temperatureReceive, size, MPI_DOUBLE,
                       nullptr, 0, MPI_DOUBLE,
                       0, MPI_COMM_WORLD);
            MPI_Gather(fReceive, size, MPI_DOUBLE,
                       nullptr, 0, MPI_DOUBLE,
                       0, MPI_COMM_WORLD);

            /// Вычисляю yn+1 и Fn+1
            // Получаю значения температуры и fReceive по Y
            MPI_Scatter(nullptr, 0, MPI_DOUBLE,
                        temperatureReceive, size, MPI_DOUBLE,
                        0, MPI_COMM_WORLD);
            MPI_Scatter(nullptr, 0, MPI_DOUBLE,
                        fReceive, size, MPI_DOUBLE,
                        0, MPI_COMM_WORLD);

            // Само вычисление
            solveThomas(fReceive, myLambdaY, size, TxLeft, TxRight, xStep, timeStep, temperatureReceive);
            restoreValues(temperatureReceive, myLambdaY, size, xStep, timeStep, fReceive);

            // Возвращаю полученные значения
            MPI_Gather(temperatureReceive, size, MPI_DOUBLE,
                       nullptr, 0, MPI_DOUBLE,
                       0, MPI_COMM_WORLD);
            MPI_Gather(fReceive, size, MPI_DOUBLE,
                       nullptr, 0, MPI_DOUBLE,
                       0, MPI_COMM_WORLD);
        }
    }

    MPI_Type_free(&columnType);
    MPI_Type_free(&matrixColumnsType);

    MPI_Finalize();

}
