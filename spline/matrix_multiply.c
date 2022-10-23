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

    // Direct
    float* alpha = malloc(sizeof(float) * size);
    float* beta = malloc(sizeof(float) * size);
    alpha[1] = -b[0] / c[0];
    beta[1] = f[0] / c[0];

    for (int i = 1; i < size - 1; ++i) {
        alpha[i + 1] = -b[i] / (a[i] * alpha[i] + c[i]);
        beta[i + 1] = (f[i] - a[i] * beta[i]) / (a[i] * alpha[i] + c[i]);
    }

    // Reverse
    float *x = malloc(sizeof(float) * size);
    int n = size - 1;
    x[n] = (f[n] - a[n] * beta[n]) / (a[n] * alpha[n] + c[n]);

    for (int i = n - 1; i > -1; i--) {
        x[i] = (alpha[i + 1] * x[i + 1] + beta[i + 1]);
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
