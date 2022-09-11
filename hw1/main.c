#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "unistd.h"
#include "matrix.h"
#include "measurements.h"

typedef struct matrix_measure_t {
    multiply_matrices_func func;
    const char* order;
} matrix_measure_t;

void measure_multiply_out(matrix_measure_t* measure, size_t repeat_time, size_t matrix_size) {
    double result = measure_multiply(*(measure->func), repeat_time, matrix_size);
    printf("%s: %f\n", measure->order, result);
}

typedef struct test_case_t {
    size_t repeat_time;
    size_t matrix_size;
    matrix_measure_t* measures;
    size_t measures_count;
} test_case_t;

void measure_multiply_test_case_out(test_case_t* test_case) {
    size_t repeat_time = test_case->repeat_time;
    size_t matrix_size = test_case->matrix_size;
    for (int i = 0; i < test_case->measures_count; ++i) {
        matrix_measure_t measure = {
                .func = test_case->measures[i].func,
                .order = test_case->measures[i].order
        };
        measure_multiply_out(&measure, repeat_time, matrix_size);
    }
}

int main() {
    srand(time(NULL));
    const size_t measures_count = 6;
    matrix_measure_t measures[] = {
            {
                .order = "ijk",
                .func = matrix_multiply_ijk
            }, {
                .order = "ikj",
                .func = matrix_multiply_ikj
            }, {
                .order = "jik",
                .func = matrix_multiply_jik
            }, {
                .order = "jki",
                .func = matrix_multiply_jki
            }, {
                .order = "kij",
                .func = matrix_multiply_kij
            }, {
                .order = "kji",
                .func = matrix_multiply_kji
            }
    };
    size_t repeat_time = 50;
    size_t matrix_size = 100;
    test_case_t test_case = {
            .measures = measures,
            .matrix_size = matrix_size,
            .repeat_time = repeat_time,
            .measures_count = measures_count
    };
    measure_multiply_test_case_out(&test_case);
}