//
// Created by ashblade on 23.10.22.
//

#include "matrix_multiply.h"
#include "stdlib.h"

static solve_result_t* createSolveResult(int size, float* x) {
    solve_result_t * result = malloc(sizeof (solve_result_t));
    result->size = size;
    result->x = x;
    return result;
}

solve_result_t *solveSequential(matrix_t *matrix) {
    matrix_t* copy = copy_new(matrix);
    int size = copy->size;
    float *a = copy->a;
    float *b = copy->b;
    float *c = copy->c;
    float *f = copy->f;
    for (int i = 1; i < size; ++i) {
        const float coefficient = a[i] / b[i - 1];
        b[i] = b[i] - coefficient * c[i - 1];
        f[i] = f[i] - coefficient * f[i - 1];
    }

    for (int i = size - 2; i > -1; i--) {
        const float coefficient = c[i] / b[i + 1];
        f[i] = f[i] - coefficient * f[i + 1];
    }

    float* x = malloc(sizeof(int) * size);

    for (int i = 0; i < size; ++i) {
        x[i] = f[i] / b[i];
    }

    return createSolveResult(size, x);
}

solve_result_t *solveParallel(matrix_t *matrix) {
    int size = matrix->size;
    float *a = matrix->a;
    float *b = matrix->b;
    float *c = matrix->c;
    float *f = matrix->f;
    for (int i = 1; i < size; ++i) {
        float coefficient = a[i] / b[i - 1];
        a[i] = -coefficient * a[i - 1];
        b[i] = b[i] - coefficient * c[i - 1];
        f[i] = f[i] - coefficient * f[i - 1];
    }

    for (int i = size - 3; i < -1; i--) {
        float coefficient = c[i] / b[i + 1];
        c[i] = -coefficient * c[i + 1];
        a[i] = a[i] - coefficient * a[i + 1];
        f[i] = f[i] - coefficient * f[i + 1];
    }
    return NULL;
}
