/**
 * @file merge_sort.c
 * @brief Implementation of merge sort algorithm
 * 
 * Merge sort is a divide and conquer algorithm.
 * It divides the input array into two halves, calls itself for the two halves, and then merges the two sorted halves.
 * The merge() function is used for merging two halves.
 * The time complexity of merge sort is O(n log n) in all cases (worst, average, and best).
 * Merge sort is stable and works well with large datasets.
 * Note however that threaded merge sort creates additional overhead and is not recommended for small datasets (combine with other algorithms).
 */
#include "common.h"

void merge(int *data, int left, int middle, int right){
    int n1 = middle - left + 1;
    int n2 = right - middle;

    // Create temp arrays
    int L[n1], R[n2];
    for(int i = 0; i < n1; i++){
        L[i] = data[left + i];
    }
    for(int i = 0; i < n2; i++){
        R[i] = data[middle + 1 + i];
    }

    // Merge the temp arrays back into data
    int i = 0;
    int j = 0;
    int k = left;
    while(i < n1 && j < n2){
        if(L[i] <= R[j]){
            data[k] = L[i];
            i++;
        }else{
            data[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[], if there are any
    while(i < n1){
        data[k] = L[i];
        i++;
        k++;
    }

    // Copy the remaining elements of R[], if there are any
    while(j < n2){
        data[k] = R[j];
        j++;
        k++;
    }
}

void merge_sort(int *data, int left, int right){
    if(left < right){
        int middle = left + (right - left) / 2;
        merge_sort(data, left, middle);
        merge_sort(data, middle + 1, right);
        merge(data, left, middle, right);
    }
}

int main(int argc, char *argv[]){
    if(argc != 3){
        printf("./merge file_path N\n");
        return -1;
    }

    char *file_name = argv[1];
    int N = atoi(argv[2]);
    int *data = create_array(N);
    fetch_values(file_name, data);

    clock_t start = clock();
    merge_sort(data, 0, N-1);
    clock_t end = clock();
    double time = (double)(end - start) / CLOCKS_PER_SEC;

    if(check_sorting(data, N)){
        printf("Sucsessfully sorted %d values\n", N);
        printf("Time: %.6f\n", time);
    }else{
        printf("Failed to sort %d values\n", N);
    }
    return 0;
}