#include <algorithm>
#include <iostream>

template <class T>
void quickSort(T* a, const long n) {
    long i = 0, j = n-1;
    T pivot = a[n / 2];

    do {
        while (a[i] < pivot) {
            i++;
        }
        while (a[j] > pivot) {
            j--;
        }
        if (i <= j) {
            std::swap(a[i], a[j]);
            i++; j--;
        }
    } while (i <= j);

#pragma omp task shared(a, j) default(none)
    {
        if (j > 0) {
            quickSort(a, j+1);
        }
    }
#pragma omp task shared(a, i, n) default(none)
    {
        if (n > i) {
            quickSort(a + i, n - i);
        }
    }
#pragma omp taskwait
}

int main(int argc, char** argv) {
    const int size = 100;
    int* array = new int[size];
    for (int i = size - 1; i >= 0; --i) {
        array[i] = i;
    }
    quickSort(array, size);
    for (int i = 0; i < size; ++i) {
        std::cout << array[i] << " ";
    }
    delete[] array;
}






