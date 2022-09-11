#include <iostream>
#include <omp.h>
#include <sstream>

#define BUFFER_SIZE 1000


int main(int argc, char** argv) {
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