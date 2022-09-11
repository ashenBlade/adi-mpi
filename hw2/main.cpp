#include <iostream>
#include <omp.h>
#include <sstream>
#include <unistd.h>
#include <memory>
#include <vector>
#include <algorithm>

#define BUFFER_SIZE 1000
#define VECTORS_COUNT 2

std::vector<int>* init_random_vectors(size_t vector_size, size_t vector_count = 2) {
    auto vectors = new std::vector<int>[vector_count];
    for (int i = 0; i < vector_count; ++i) {
        vectors[i] = std::vector<int>(vector_size);
    }

    return vectors;
}

long sum_vectors_single(size_t vector_size, size_t vectors_count = VECTORS_COUNT) {
    auto vectors = init_random_vectors(vector_size, vectors_count);
    long total_sum = 0;

    omp_set_nested(true);
    for (int i = 0; i < vectors_count; ++i) {
        auto vector = vectors[i];
        int n;
        for (int j : vector) {
            n = j;
        }

        total_sum = n;
    }

    delete[] vectors;
    return total_sum;
}

long sum_vectors_parallel(size_t vector_size, size_t vectors_count = VECTORS_COUNT) {
    auto vectors = init_random_vectors(vector_size, vectors_count);
    long total_sum = 0;

    omp_set_nested(true);
#pragma omp parallel for shared(vectors_count, vectors) reduction(+:total_sum) default(none)
    for (int i = 0; i < vectors_count; ++i) {
        auto vector = vectors[i];
        int n;
#pragma omp parallel for reduction(+:n) private(vector) default(none)
        for (int j : vector) {
            n = j;
        }

        total_sum = n;
    }

    delete[] vectors;
    return total_sum;
}


int main(int argc, char** argv) {
    auto start = omp_get_wtime();
    int vectors_count = 100;
    int vector_size = 100000;
    auto result = sum_vectors_parallel(vector_size, vectors_count);
    auto end = omp_get_wtime();
    std::cout << "Execution time for parallel: " << (end - start) << std::endl;
    
    start = omp_get_wtime();
    result = sum_vectors_single( vector_size, vectors_count);
    end = omp_get_wtime();
    std::cout << "Execution time for single: " << end - start << std::endl;
}

void rewritten_main(int argc, char** argv) {
    std::cout << "Processors count: " << omp_get_num_procs() << "\n";
    int a[BUFFER_SIZE],
            b[BUFFER_SIZE],
            c[BUFFER_SIZE],
            d[BUFFER_SIZE];
#pragma omp parallel for shared(a, b, c, d, std::cout) default(none)
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        a[i] = i;
        b[i] = i;
        d[i] = 2 * i;
        std::stringstream ss;
        ss << "Current thread number:\t" << omp_get_thread_num() << "\n";
        ss << "Total threads number:\t" << omp_get_num_threads() << "\n";
        ss << "-----------------------------\n";
        std::cout << ss.str();
    }
#pragma omp parallel for shared(a, b, c, d) default(none)
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        c[i] = a[i] + b[i] + d[i];
    }

    std::cout << "a[" << BUFFER_SIZE / 2 << "] = " << a[BUFFER_SIZE / 2] << std::endl;
}