//
// Created by ashblade on 23.10.22.
//

#ifndef SPLINE_MATRIX_MULTIPLY_H
#define SPLINE_MATRIX_MULTIPLY_H

#include "matrix.h"

typedef struct multiply_result {
    float* x;
    int length;
} multiply_result_t;

multiply_result_t* solve_sequential(matrix_t* matrix);
multiply_result_t* solve_parallel(matrix_t* matrix);

#endif //SPLINE_MATRIX_MULTIPLY_H
