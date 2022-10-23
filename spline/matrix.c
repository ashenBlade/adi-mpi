//
// Created by ashblade on 23.10.22.
//

#include "matrix.h"
#include "stdlib.h"
#include "time.h"

matrix_t *createRandom(int size) {
    srand(time(NULL));
    matrix_t* empty = createEmpty(size);
    for (int i = 0; i < size; ++i) {
        empty->a[i] = rand();
        empty->b[i] = rand();
        empty->c[i] = rand();
        empty->f[i] = rand();
    }
    return empty;
}

void copy(matrix_t *from, matrix_t *to) {
    to->size = from->size;
    for (int i = 0; i < to->size; ++i) {
        to->a[i] = from->a[i];
        to->b[i] = from->b[i];
        to->c[i] = from->c[i];
        to->f[i] = from->f[i];
    }
}

matrix_t* copy_new(matrix_t* from) {
    matrix_t* result = createEmpty(from->size);
    for (int i = 0; i < from->size; ++i) {
        result->a[i] = from->a[i];
        result->b[i] = from->b[i];
        result->c[i] = from->c[i];
        result->f[i] = from->f[i];
    }
    return result;
}

void destruct(matrix_t *matrix) {
    free(matrix->a);
    free(matrix->b);
    free(matrix->c);
    free(matrix->f);
    matrix->size = -1;
}

matrix_t *createEmpty(int size) {
    matrix_t* matrix = malloc(sizeof(matrix_t));
    matrix->a = malloc(size * sizeof(float));
    matrix->b = malloc(size * sizeof(float));
    matrix->c = malloc(size * sizeof(float));
    matrix->f = malloc(size * sizeof(float));
    matrix->size = size;
    return matrix;
}
