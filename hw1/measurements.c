//
// Created by ashblade on 07.09.22.
//

#include "measurements.h"

matrix_t* matrix_multiply_ijk(matrix_t* left, matrix_t* right) {
    if (left->size != right->size) {
        return NULL;
    }
    matrix_t* result = create_zero_matrix(left->size);
    size_t size = left->size;
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            for (int k = 0; k < size; ++k) {
                result->matrix[i][j] += left->matrix[i][k] * right->matrix[k][j];
            }
        }
    }
    return result;
}

matrix_t* matrix_multiply_ikj(matrix_t* left, matrix_t* right) {
    if (left->size != right->size) {
        return NULL;
    }
    matrix_t* result = create_zero_matrix(left->size);
    size_t size = left->size;
    for (int i = 0; i < size; ++i) {
        for (int k = 0; k < size; ++k) {
            for (int j = 0; j < size; ++j) {
                result->matrix[i][j] += left->matrix[i][k] * right->matrix[k][j];
            }
        }
    }
    return result;
}

matrix_t* matrix_multiply_jki(matrix_t* left, matrix_t* right) {
    if (left->size != right->size) {
        return NULL;
    }
    matrix_t* result = create_zero_matrix(left->size);
    size_t size = left->size;
    for (int j = 0; j < size; ++j) {
        for (int k = 0; k < size; ++k) {
            for (int i = 0; i < size; ++i) {
                result->matrix[i][j] += left->matrix[i][k] * right->matrix[k][j];
            }
        }
    }
    return result;
}

matrix_t* matrix_multiply_jik(matrix_t* left, matrix_t* right) {
    if (left->size != right->size) {
        return NULL;
    }
    matrix_t* result = create_zero_matrix(left->size);
    size_t size = left->size;
    for (int j = 0; j < size; ++j) {
        for (int i = 0; i < size; ++i) {
            for (int k = 0; k < size; ++k) {
                result->matrix[i][j] += left->matrix[i][k] * right->matrix[k][j];
            }
        }
    }
    return result;
}

matrix_t* matrix_multiply_kij(matrix_t* left, matrix_t* right) {
    if (left->size != right->size) {
        return NULL;
    }
    matrix_t* result = create_zero_matrix(left->size);
    size_t size = left->size;
    for (int k = 0; k < size; ++k) {
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                result->matrix[i][j] += left->matrix[i][k] * right->matrix[k][j];
            }
        }
    }
    return result;
}

matrix_t* matrix_multiply_kji(matrix_t* left, matrix_t* right) {
    if (left->size != right->size) {
        return NULL;
    }
    matrix_t* result = create_zero_matrix(left->size);
    size_t size = left->size;
    for (int k = 0; k < size; ++k) {
        for (int j = 0; j < size; ++j) {
            for (int i = 0; i < size; ++i) {
                result->matrix[i][j] += left->matrix[i][k] * right->matrix[k][j];
            }
        }
    }
    return result;
}


double measure_multiply(multiply_matrices_func func, size_t repeat_time, size_t matrix_size) {
    double total_time = 0;
    matrix_t* a = create_random_matrix(matrix_size);
    matrix_t* b = create_random_matrix(matrix_size);
    for (int i = 0; i < repeat_time; ++i) {
        clock_t start = clock();
        matrix_t* result = func(a, b);
        clock_t end = clock();
        total_time += (double) (end - start);
        matrix_destruct(result);
    }
    matrix_destruct(a);
    matrix_destruct(b);
    return total_time / (double)(repeat_time * CLOCKS_PER_SEC);
}
