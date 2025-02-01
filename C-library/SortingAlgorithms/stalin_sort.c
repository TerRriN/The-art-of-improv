/**
 * @file stalin_sort.c
 * @brief Sorts an array of integers using the Stalin sort algorithm
 * 
 * Stalin sort is a sorting algorithm that removes all elements that are not in order.
 * It is not a practical sorting algorithm, but it is a fun one.
 * Worst case scenario: if the first element is the largest, it will remove all elements except the first one.
 * Time complexity: O(n)
 */

#include "common.h"

int stalin(int *data, int N){
    int si = 0; //sorted index
    for(int i = 1; i < N; i++){
        if(data[(si)] <= data[i]){
            si++;
            data[(si)] = data[i];
        }
    }
    return si;
}

int main(int argc, char *argv[]){
    if(argc != 3){
        printf("./stalin_sort file_path n_values\n");
        return -1;
    }

    char *file_name = argv[1]; 
    int N = atoi(argv[2]);
    int *data = create_array(N);
    fetch_values(file_name, data);

    clock_t start = clock();
    int si = stalin(data, N);
    clock_t end = clock();
    double tmp = end - start;
    double elapsed = tmp/CLOCKS_PER_SEC;

    if(check_sorting(data, si+1)){
        printf("Sucsessfully sorted %d values, only %d returned\n", N, si+1);
        printf("Time: %.6f\n", elapsed);
    }else{
        printf("Failed to sort %d values\n", N);
    }
    return 0;
}