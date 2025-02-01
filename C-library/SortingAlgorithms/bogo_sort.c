/**
 * @file bogo_sort.c
 * @brief Implementation of the bogo sort algorithm.
 * @warning This algorithm is highly inefficient and should not be used for any real-world applications.
 * 
 * Bogo Sort is by far the least efficient sorting algorithm. It is not suitable for any real-world applications.
 * The algorithm "sorts" an array by randomly shuffling the elements until they are in the correct order.
 * The average time complexity of Bogo Sort is O((N+1)!).
 */

#include "common.h"

void bogo_sort(int *data, int N) {
    while (!check_sorting(data, N)) {
        for (int i = 0; i < N; i++) {
            int j = rand() % N;
            int temp = data[i];
            data[i] = data[j];
            data[j] = temp;
        }
    }
}

int main(char argc, char *argv[]) {
    if(argc != 3){
        printf("./bogo file_path n_values\n");
        return -1;
    }

    char *file_name = argv[1];
    int N = atoi(argv[2]);
    int *data = create_array(N);
    fetch_values(file_name, data);

    clock_t start = clock();
    bogo_sort(data, N);
    clock_t end = clock();
    double tmp = end - start;
    double elapsed = tmp/CLOCKS_PER_SEC;

    if(check_sorting(data, N)){
        printf("Sucsessfully sorted %d values\n", N);
        printf("Time: %.6f\n", elapsed);
    }else{
        printf("Failed to sort %d values\n", N);
    }
}