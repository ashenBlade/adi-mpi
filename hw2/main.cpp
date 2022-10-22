#include <iostream>
#include <omp.h>
#include <sstream>
#include <unistd.h>
#include <memory>
#include <vector>
#include "vectors_summarizing.h"
#include "matrix.h"
#include <string>

using exec_time = double;

struct ThreadTestResult {
    int threadsCount;
    exec_time standard;
    exec_time band;
    exec_time block;
};

struct TestResult {
    int size;
    double sequential;
    std::vector<ThreadTestResult> parallelResults;
};

std::vector<TestResult>* makeTests();

void test_parallel_mode();

std::vector<TestResult> *makeTests() {
    auto vector = new std::vector<TestResult>();
    auto matrixSizes = {500, 1000, 1500, 2000, 2500, 3000};
    auto threadsCounts = {2, 4, 6, 8};
    for (const auto &size: matrixSizes) {
        auto result = TestResult();
        result.size = size;
        auto left = std::unique_ptr<Matrix>(Matrix::createRandomMatrix(size));
        auto right = std::unique_ptr<Matrix>(Matrix::createRandomMatrix(size));
        result.sequential = left->multiplySequential(*right);
        for (const auto &threads: threadsCounts) {
            auto threadResult = ThreadTestResult();
            threadResult.threadsCount = threads;
            threadResult.standard = left->multiplyStandard(*right, threads);
            threadResult.band = left->multiplyBand(*right, threads);
            threadResult.block = left->multiplyBlock(*right, threads);
            result.parallelResults.push_back(threadResult);
            std::cerr << "Done: " << size << " size, " << threads << " threads\n";
        }
        vector->push_back(result);
    }
    return vector;
}

void showResults(const std::vector<TestResult>& results) {
    for (const auto &result: results) {
        std::cout << "---------------------------------------------\n";
        std::cout << "Size: " << result.size << "\n\n";
        std::cout << "Sequential: " << result.sequential << "\n\n";
        for (const auto &parallelResult: result.parallelResults) {
            std::cout << "Threads: " << parallelResult.threadsCount << "\n";
            std::cout << "\tStandard:\n\t\tTime: " << parallelResult.standard << "\n\t\t" << "Acceleration: " <<  result.sequential / parallelResult.standard << "\n";
            std::cout << "\tBand:\n\t\tTime: " << parallelResult.band << "\n\t\t" << "Acceleration: " <<  result.sequential / parallelResult.band << "\n";
            std::cout << "\tBlock:\n\t\tTime: " << parallelResult.block << "\n\t\t" << "Acceleration: " <<  result.sequential / parallelResult.block << "\n\n";
        }
    }
}



int main(int argc, char** argv) {
    int n;
#pragma omp parallel private(n)
    {
        n = 1;
#pragma omp master
        {
            n = 2;
        };
        std::cout << "First n: " + std::to_string(n) + '\n';
#pragma omp barrier
#pragma omp master
        {
            n = 3;
        };
        std::cout << "Second n: " + std::to_string(n) + '\n';
    }
}

void test_parallel_mode() {
    if (omp_in_parallel()) {
        std::cout << "Parallel\n";
    } else {
        std::cout << "Sequential\n";
    }
#pragma omp parallel
    {
#pragma omp master
        {
            if (omp_in_parallel()) {
                std::cout << "Parallel\n";
            } else {
                std::cout << "Sequential\n";
            }
        }
    };

}







