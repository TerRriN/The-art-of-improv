#include "common.h"
int *create_array(int N){
    return malloc(sizeof(int)*N);
}

void fetch_values(char *file_name, int *data){
    FILE *file;
    file = fopen(file_name, "r");
    int value = 0;
    int i = 0;
    while(!feof(file)){
        fscanf(file, "%d", &value);
        data[i] = value;
        i++;
    }
    fclose(file);
}

bool check_sorting(int *data, int N){
    for(int i = 0; i < N-1; i++){
        if(data[i] > data[i+1]){
            return false;
        }
    }
    return true;
}

bool check_input(int argc, char *argv[]){
    if(argc != 3){
        printf("./quick file_path n_values\n");
        if(argv[1] != NULL){
            printf("File path: %s\n", argv[1]);
        }
        if(argv[2] != NULL){
            printf("Number of values: %s\n", argv[2]);
        }
        return false;
    }
    return true;
}