//
// Created by ashblade on 23.10.22.
//

#ifndef SPLINE_MATRIX_H
#define SPLINE_MATRIX_H

typedef struct matrix {
    float* a;
    float* b;
    float* c;
    float* f;
    int size;
} matrix_t;

matrix_t createRandom(int size);
matrix_t copy(matrix_t* matrix);
void destruct(matrix_t* matrix);

#endif //SPLINE_MATRIX_H
