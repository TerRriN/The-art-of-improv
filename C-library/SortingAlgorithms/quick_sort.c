/**
 * @file quick_sort.c
 * @brief Implementation of the Quick Sort algorithm.
 * 
 * Quick Sort is a highly efficient sorting algorithm and is based on partitioning of array of data into smaller arrays.
 * A large array is partitioned into two arrays, based on a chosen pivot element all elements smaller than the pivot 
 * are moved to the left of the pivot, and all greater elements are moved to the right.
 * Quick Sort partitions an array and then calls itself recursively twice to sort the two resulting subarrays.
 * This algorithm is quite efficient for large-sized data sets as its average and worst-case complexity are O(n log n) and O(n^2), respectively.
 * 
 * Key Points:
 * - Quick Sort is a comparison sort and is not a stable sort.
 * - It is an in-place sort (it doesn't require any extra storage).
 * - The choice of pivot can greatly affect the performance of the algorithm.
 * - Common strategies for choosing a pivot include picking the first element, the last element, the middle element, or a random element.
 * - This implementation uses random pivots.
 */

#include "common.h"

/**
 * Implemented with https://www.geeksforgeeks.org/quick-sort-in-c/
 */
void swap(int *x, int *y){
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

int partition(int *data, int left, int right) {
    int pivot_index = left + rand() % (right - left + 1);
    int pivot = data[pivot_index];
    swap(&data[pivot_index], &data[right]);
    int i = left - 1;

    for (int j = left; j <= right - 1; j++) {
        if (data[j] < pivot) {
            i++;
            swap(&data[i], &data[j]);
        }
    }
    swap(&data[i + 1], &data[right]);
    return i + 1;
}

void quick_sort(int *data, int left, int right) {
    if (left < right) {
        int pi = partition(data, left, right);

        quick_sort(data, left, pi - 1);
        quick_sort(data, pi + 1, right);
    }
}

int main(int charc, char *argv[]){
    if(!check_input(charc, argv)){
        return 1;
    }

    char *file_name = argv[1];
    int N = atoi(argv[2]);
    int *data = create_array(N);
    fetch_values(file_name, data);

    clock_t start = clock();
    quick_sort(data, 0, N-1);
    clock_t end = clock();
    double tmp = end - start;
    double elapsed = tmp/CLOCKS_PER_SEC;
    
    if(check_sorting(data, N)){
        printf("Sucsessfully sorted %d values\n", N);
        printf("Time: %.6f\n", elapsed);
    }else{
        printf("Failed to sort %d values\n", N);
    }
    return 0;
}