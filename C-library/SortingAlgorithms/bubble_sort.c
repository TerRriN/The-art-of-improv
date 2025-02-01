#include "common.h"

void bubble_sort(int *data, int N){
    for(int i = 0; i < N - 1; i++){
        for(int j = 0; j < N - i - 1; j++){
            if(data[j] > data[j + 1]){
                int temp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = temp;
            }
        }
    }
}

int main(int argc, char *argv[]){
    if(argc != 3){
        printf("./bubble file_path N\n");
        return -1;
    }

    char *file_name = argv[1];
    int N = atoi(argv[2]);
    int *data = create_array(N);
    fetch_values(file_name, data);

    clock_t start = clock();
    bubble_sort(data, N);
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