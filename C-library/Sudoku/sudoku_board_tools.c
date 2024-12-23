#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> //Must include -lm when compiling
#include <stdbool.h>

int N;
int sqrt_N;

void sud_display_translator_ss(char board[N][N], char *file_name){
    FILE *file;
    file = fopen(file_name, "r");
    char c = ' ';
    int i = 0, j = 0;
    char prev = ' ';

    while(c != EOF){
        c = fgetc(file);
        if(c == '\n' && prev == '-'){
            j = 0;
        }else if(c == '\n'){
            i++;
            j = 0;
        }else if(c == '.'){
            board[i][j] = '0';
            j++;
        }else if(c >= '1' && c <= '9'){
            board[i][j] = c;
            j++;
        }
        prev = c;
    }
}

void sud_display_line_std(){ 
    int line_length = (N*sqrt_N)+sqrt_N+1;
    for(int i = 0; i < line_length; i++){
        printf("-");
    }
    printf("\n");
}

void sud_display_board_std(char board[N][N]){
    int x = 0;
    int line = 0;
    int wall = 0;

    sud_display_line_std();
    printf("|");
    for(int i = 0; i < N; i++){
        if(line == sqrt_N){
            sud_display_line_std();
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
    sud_display_line_std();
}

int main(int charc, char *argv[]){
    //9x9
    N = 9;
    sqrt_N = sqrt(N);
    char board[N][N];
    char * file_name = "board.ss";
    sud_display_translator_ss(board, file_name);
    sud_display_board_std(board);
    return 0;
}