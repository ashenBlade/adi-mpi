//
// Created by ashblade on 11.09.22.
//

#include <algorithm>
#include <vector>
#include <memory>
#include <unistd.h>
#include <sstream>
#include <omp.h>
#include <iostream>
#include "vectors_summarizing.h"

long sum_vectors_parallel(const std::vector<std::vector<int>>& vectors) {
    long total_sum = 0;

    omp_set_nested(true);
    auto vectors_count = vectors.size();
#pragma omp parallel for shared(vectors_count, vectors) reduction(+:total_sum) default(none)
    for (auto&& vector: vectors) {
        int n;
#pragma omp parallel for reduction(+:n) shared(vector) default(none)
        for (const int j : vector) {
            n = j;
        }

        total_sum = n;
    }

    return total_sum;
}

long sum_vectors_single(const std::vector<std::vector<int>>& vectors) {
    long total_sum = 0;
    auto vectors_count = vectors.size();

    omp_set_nested(true);
    for (int i = 0; i < vectors_count; ++i) {
        auto vector = vectors[i];
        int n;
        for (int j : vector) {
            n = j;
        }

        total_sum = n;
    }
    return total_sum;
}

