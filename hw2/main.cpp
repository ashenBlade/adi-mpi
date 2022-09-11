#include <iostream>
#include <omp.h>
#include <sstream>
#include <unistd.h>
#include <memory>
#include <vector>
#include "vectors_summarizing.h"


static std::vector<std::vector<int>>* init_random_vectors(size_t vectors_count, size_t vector_size) {
    auto vectors = new std::vector<std::vector<int>>[vectors_count];
    auto start_nested = omp_get_nested();
    omp_set_nested(true);
#pragma omp parallel for shared(vectors) default(none)
    for (auto &&vector: *vectors) {
#pragma omp parallel for shared(vector) private(vector_size) default(none)
        for (int i = 0; i < vector_size; ++i) {
            vector[i] = rand();
        }
    }
    omp_set_nested(start_nested);
    return vectors;
}



int main(int argc, char** argv) {
    srand(time(nullptr));
    int vectors_count = 1000;
    int vector_size = 20000;
    auto vectors = std::unique_ptr<std::vector<std::vector<int>>>(init_random_vectors(vectors_count, vector_size));

    auto start = omp_get_wtime();
    sum_vectors_parallel(*vectors);
    auto end = omp_get_wtime();
    std::cout << "Execution time for parallel: " << (end - start) << std::endl;
    
    start = omp_get_wtime();
    sum_vectors_single(*vectors);
    end = omp_get_wtime();
    std::cout << "Execution time for single: " << end - start << std::endl;

}


void rewritten_main(int argc, char** argv) {
#define BUFFER_SIZE 10000
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