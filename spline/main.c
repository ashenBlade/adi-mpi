#include "stdio.h"
#include "matrix_multiply.h"

int main() {
    matrix_t* matrix = createRandom(10);
    solve_result_t* result = solveSequential(matrix);
    for (int i = 0; i < result->size; ++i) {
        printf("%f ", result->x[i]);
    }
    return 0;
}
