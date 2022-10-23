//
// Created by ashblade on 23.10.22.
//

#ifndef SPLINE_MATRIX_MULTIPLY_H
#define SPLINE_MATRIX_MULTIPLY_H

#include "matrix.h"

typedef struct solve_result {
    float* x;
    int size;
} solve_result_t;

solve_result_t* solveSequential(matrix_t* matrix);
solve_result_t* solveParallel(matrix_t* matrix);

#endif //SPLINE_MATRIX_MULTIPLY_H
