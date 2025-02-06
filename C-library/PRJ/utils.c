#include "utils.h"

bool DuplicateNumbersinRow(char board[N][N], int y, int x){
    char comparator = board[y][x];
    for(int i = 0; i < N; i++){
        if(board[y][i] == comparator && i != x){
            return true;
        }
    }
    return false;
}

bool DuplicateNumbersinCol(char board[N][N], int y, int x){
    char comparator = board[y][x];
    for(int i = 0; i < N; i++){
        if(board[i][x] == comparator && i != y){
            return true;
        }
    }
    return false;
}

bool DuplicateNumbersinBox(char board[N][N], int y, int x){
    int boxSize = sqrt_N;
    int start_y = y - y % boxSize;
    int start_x = x - x % boxSize;
    char comparator = board[y][x];
    int tmp = 0;

    for(int i = 0; i < boxSize; i++){
        for(int j = 0; j < boxSize; j++){
            if(board[i + start_y][j + start_x] == comparator){
                tmp++;
            }
        }
    }
    if(tmp > 1){
        return true;
    }
    return false;
}

bool ValidateBoard(char board[N][N], int y, int x){
    if(DuplicateNumbersinBox(board, y, x))      return false; //BOX
    if(DuplicateNumbersinCol(board, y, x))      return false; //COL
    if(DuplicateNumbersinRow(board, y, x))      return false; //ROW
    return true;
}

bool check_entire_board(char board[N][N]){
    bool x_flag = true;
    bool y_flag = true;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            x_flag = false;
            y_flag = false;
            for(int k = 0; k < N; k++){
                if(board[i][j] == '0') return false;
                if(board[j][i] == '0') return false;
                if(board[i][j] == (k+1) + '0'){
                    if(x_flag){
                        return false;
                    }
                    x_flag = true;
                }
                if(board[j][i] == (k+1) + '0'){
                    if(y_flag){
                        return false;
                    }
                    y_flag = true;
                }
            }
        }
    }
    return true;
}

/* Current hardcoded solution:
9 5 1 7 8 2 4 3 6 
8 3 4 1 9 6 2 7 5 
2 7 6 5 4 3 1 9 8 
7 4 8 3 5 1 6 2 9 
3 6 9 4 2 7 8 5 1 
5 1 2 9 6 8 7 4 3 
4 8 5 2 1 9 3 6 7 
1 2 7 6 3 5 9 8 4 
6 9 3 8 7 4 5 1 2
*/
bool compare_to_solution(char board[N][N]){
    char *solution = "951782436834196275276543198748351629369427851512968743485219367127635984693874512";
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(board[i][j] != solution[i*N+j]){
                return false;
            }
        }
    }
    return true;
}