//
// Created by ashblade on 07.09.22.
//

#ifndef C_MATRIX_H
#define C_MATRIX_H


#ifdef __cplusplus
extern "C" {
#endif

#include "unistd.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>


typedef struct matrix_t {
    size_t size;
    int **matrix;
} matrix_t;

typedef int(*get_value_func)(void);

matrix_t *create_zero_matrix(size_t size);

matrix_t *create_random_matrix(size_t size);

void matrix_destruct(matrix_t *matrix);


#ifdef __cplusplus
}
#endif

#endif //C_MATRIX_H
