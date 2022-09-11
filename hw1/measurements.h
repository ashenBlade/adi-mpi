//
// Created by ashblade on 07.09.22.
//

#ifndef C_MEASUREMENTS_H
#define C_MEASUREMENTS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "matrix.h"

matrix_t* matrix_multiply_ikj(matrix_t* left, matrix_t* right);
matrix_t* matrix_multiply_ijk(matrix_t* left, matrix_t* right);
matrix_t* matrix_multiply_jki(matrix_t* left, matrix_t* right);
matrix_t* matrix_multiply_jik(matrix_t* left, matrix_t* right);
matrix_t* matrix_multiply_kij(matrix_t* left, matrix_t* right);
matrix_t* matrix_multiply_kji(matrix_t* left, matrix_t* right);

typedef matrix_t*(*multiply_matrices_func)(matrix_t*, matrix_t*);

double measure_multiply(multiply_matrices_func func, size_t repeat_time, size_t matrix_size);

#ifdef __cplusplus
}
#endif

#endif //C_MEASUREMENTS_H
