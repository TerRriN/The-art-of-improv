#include "common.h"

int N;
int sqrt_N;

double get_wall_time() {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return (double)time.tv_sec + (double)time.tv_nsec / 1e9;
}

void print_board(char board[N][N]){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf(" %d ", board[i][j] - '0');
        }
        printf("\n");
    }
    printf("\n");
}

void print_possibilities(char poss[N][N][N]){
    int x = 0;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            printf("x: %d, y: %d: [ ", j, i);
            for(int k = 0; k < N; k++){
                x = poss[i][j][k] - '0';
                if(x != 0) printf("%d ", x);
            }
            printf("]\n");
        }
    }
}


void print_precise_possibilities(char poss[N][N][N], int y, int x){
    int x_val = 0;
    printf("x: %d, y: %d: [ ", x, y);
    for(int k = 0; k < N; k++){
        x_val = poss[y][x][k] - '0';
        if(x_val != 0) printf("%d ", x_val);
    }
    printf("]\n");
}

// Helper function to sud_display_board_std
void sud_display_line(int line_length){
    for(int i = 0; i < line_length; i++){
        printf("-");
    }
    printf("\n");
}

void sud_display_board_std(char board[N][N]){
    int x = 0;
    int line = 0;
    int wall = 0;
    int line_length = (N*sqrt_N)+sqrt_N+1;

    sud_display_line(line_length);
    printf("|");
    for(int i = 0; i < N; i++){
        if(line == sqrt_N){
            sud_display_line(line_length);
            line = 0;
        }
        for(int j = 0; j < N; j++){
            if(wall == sqrt_N){
                wall = 0;
                printf("|");
            }
            x = board[i][j] - '0';
            if(x != 0){
                printf(" %d ", x);
            }else{
                printf("   ");
            }
            wall++;
        }
        printf("|");
        printf("\n");
        line++;
    }
    sud_display_line(line_length);
}

void sud_display_board_giant(char board[N][N]){
    int x = 0;
    int line = 0;
    int wall = 0;
    int line_length = (N*sqrt_N)+sqrt_N+1;

    //One cell: 

    sud_display_line(line_length);
    printf("|");
    for(int i = 0; i < N; i++){
        if(line == sqrt_N){
            sud_display_line(line_length);
            line = 0;
        }
        for(int j = 0; j < N; j++){
            if(wall == sqrt_N){
                wall = 0;
                printf("|");
            }
            x = board[i][j] - '0';
            if(x >= 10){
                printf(" %d ", x);
            }else if(x != 0){
                printf("  %d ", x);
            }else{
                printf("    ");
            }
            wall++;
        }
        printf("|");
        printf("\n");
        line++;
    }
    sud_display_line(line_length);
}