#include "set_board.h"

void set_numbers_ss(char board[N][N], char *file_name){
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
    fclose(file);
}


void set_numbers_txt(char board[N][N], char *file_name){
    FILE *file;
    file = fopen(file_name, "r");
    char c;
    int i = 0, j = 0;
    int val = 0;

    while((c = fgetc(file)) != EOF){
        if(c == ' '){
            board[i][j] = val + '0';
            val = 0;
            j++;
        }else if(c == '\n'){
            board[i][j] = val + '0';
            val = 0;
            i++;
            j = 0;
        }else{
            val = val*10 + (c - '0');
        }
    }
    fclose(file);
}

void set_numbers(char board[N][N], char *file_name){
    char *ext = strrchr(file_name, '.');
    if(strcmp(ext, ".ss") == 0){
        set_numbers_ss(board, file_name);
    }else if(strcmp(ext, ".txt") == 0){
        set_numbers_txt(board, file_name);
    }
}