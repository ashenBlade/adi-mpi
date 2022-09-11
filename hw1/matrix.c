//
// Created by ashblade on 07.09.22.
//

#include "matrix.h"

matrix_t *create_matrix(size_t size, get_value_func get_value) {
    matrix_t* matrix = malloc(sizeof(matrix_t));
    matrix->size = size;
    int** rows = malloc(size * sizeof(int*));
    for (size_t i = 0; i < size; i++)
    {
        int* column = malloc(size * sizeof(int*));
        for (size_t j = 0; j < size; j++)
        {
            column[j] = get_value();
        }
        rows[i] = column;
    }
    matrix->matrix = rows;
    return matrix;
}

int get_zero() { return 0; }

matrix_t* create_zero_matrix(size_t size) { return create_matrix(size, get_zero); }

matrix_t* create_random_matrix(size_t size) { return create_matrix(size, rand); }

void clear_matrix(matrix_t* matrix) {
    for (int i = 0; i < matrix->size; ++i) {
        free(matrix->matrix[i]);
    }
    free(matrix->matrix);
    matrix->size = 0;
}


void matrix_destruct(matrix_t* matrix) {
    clear_matrix(matrix);
}

