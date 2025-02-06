#include "logical_solver.h"

int count_unassigned(char board[N][N]){
    int N_unAssign = 0;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(board[i][j] == '0'){
                N_unAssign++;
            }
        }
    }
    return N_unAssign;
}

unAssigned_t **set_unassigned(char board[N][N], int N_unAssign){
    int k = 0;
    unAssigned_t **unAssignInd = calloc(N_unAssign, sizeof(unAssigned_t*));
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(board[i][j] == '0'){
                unAssigned_t *tmp = calloc(1, sizeof(unAssigned_t));
                tmp->x = j;
                tmp->y = i;
                unAssignInd[k] = tmp;
                k++;
            }
        }
    }
    return unAssignInd;
}

void destroy_unassigned(unAssigned_t **unAssignInd, int N_unAssign){
    for(int i = 0; i < N_unAssign; i++){
        free(unAssignInd[i]);
    }
    free(unAssignInd);
}

void set_possibilities(char board[N][N], char possibilities[N][N][N]){
    int cell_count = 0;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(board[i][j] == '0'){
                for(int val = 1; val < (N+1); val++){
                    board[i][j] = val + '0';
                    if(ValidateBoard(board, i, j)){
                        possibilities[i][j][val-1] = val + '0';
                    }else{
                        possibilities[i][j][val-1] = '0';
                    }
                    board[i][j] = '0';
                }
            }else{
                for(int val = 0; val < N; val++){
                    possibilities[i][j][val] = '0';
                }
            }
            cell_count++;
        }
    }
}


void new_update_possibilities(char board[N][N], char poss[N][N][N]){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(board[i][j] == '0'){
                for(int val = 1; val < (N+1); val++){
                    board[i][j] = val + '0';
                    if(ValidateBoard(board, i, j)){
                        poss[i][j][val-1] = val + '0';
                    }else{
                        poss[i][j][val-1] = '0';
                    }
                    board[i][j] = '0';
                }
            }else{
                for(int val = 0; val < N; val++){
                    poss[i][j][val] = '0';
                }
            }
        }
    }
}

int the_lucky_one(char board[N][N], char poss[N][N][N]){
    int lucky = 0;
    int poss_count = 0;
    int poss_val = 0;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            poss_count = 0;
            for(int k = 0; k < N; k++){
                if(poss[i][j][k] != '0'){ // If cell can set the possible value
                    poss_count++;
                    poss_val = poss[i][j][k];
                }
            }
            if(poss_count == 1){
                board[i][j] = poss_val;
                lucky++;
            }
        }
    }
    if(lucky > 0){
        new_update_possibilities(board, poss);
    }
    return lucky;
}

/**
 * @brief Helper function for the_easy_one
*/
int the_easy_one_helper(char board[N][N], char poss[N][N][N], int boxSize, int start_y, int start_x, int val){
    int count = 0;
    int new_x, new_y;
    char new_val;
    for(int i = 0; i < boxSize; i++){
        for(int j = 0; j < boxSize; j++){
            int x = j + start_x;
            int y = i + start_y;
            if(poss[y][x][val-1] != '0'){ // If cell can set the possible value
                new_x = x;
                new_y = y;
                new_val = poss[y][x][val-1];
                count++;
            }
        }
    }
    if(count == 1){
        board[new_y][new_x] = new_val;
        return 1;
    }
    return 0;
}


int the_easy_one(char board[N][N], char poss[N][N][N]){
    int count = 0;
    int boxSize = sqrt_N;
    bool changed = true;
    while(changed){
        changed = false;
        for(int box = 0; box < N; box++){
            int start_x = (box % boxSize) * boxSize;
            int start_y = (box / boxSize) * boxSize;

            int local_changed = 0;
            for(int val = 1; val < (N+1); val++){
                local_changed = the_easy_one_helper(board, poss, boxSize, start_y, start_x, val);
                if(local_changed){
                    count++;
                    changed = true;
                }
            }
        }
        new_update_possibilities(board, poss);
    }
    return count;
}

/**
 * @brief Helper function for pairs
*/
void pairs_helper(char poss[N][N][N], int boxSize, int start_y, int start_x, int val, pair_t *pair){
    int count = 0;
    int new_x[2];
    int new_y[2];

    for(int i = 0; i < boxSize; i++){
        for(int j = 0; j < boxSize; j++){
            int x = j + start_x;
            int y = i + start_y;
            if(poss[y][x][val-1] != '0'){ // If cell can set the possible value
                if(count < 2){
                    new_x[count] = x;
                    new_y[count] = y;
                }
                count++;
            }
        }
    }
    if(count == 2){ // Set pair
        pair->x1 = new_x[0];
        pair->x2 = new_x[1];
        pair->y1 = new_y[0];
        pair->y2 = new_y[1];
        pair->val = val;
    }
}

int pairs(char poss[N][N][N]){
    int count = 0;
    int boxSize = sqrt_N;
    pair_t **pairs = calloc(N, sizeof(pair_t*));
    for(int i = 0; i < N; i++){
        pairs[i] = calloc(1,sizeof(pair_t));
    }

    for(int box = 0; box < N; box++){
        int start_x = (box % boxSize) * boxSize;
        int start_y = (box / boxSize) * boxSize;
        for(int val = 1; val < (N+1); val++){
            pairs_helper(poss, boxSize, start_y, start_x, val, pairs[val-1]);
        }

        for(int i = 0; i < N; i++){
            if(pairs[i]->val == 0) continue;
            for(int j = i+1; j < N; j++){ // j = i+1 to avoid duplicates
                if(pairs[j]->x1 == pairs[i]->x1 
                && pairs[j]->x2 == pairs[i]->x2 
                && pairs[j]->y1 == pairs[i]->y1 
                && pairs[j]->y2 == pairs[i]->y2){
                    for(int k = 0; k < N; k++){
                        if(k != pairs[i]->val-1 && k != pairs[j]->val-1){
                            poss[pairs[i]->y1][pairs[i]->x1][k] = '0';
                            poss[pairs[i]->y2][pairs[i]->x2][k] = '0';
                            count++;
                        }
                    }
                }
            }
            pairs[i]->val = 0; // Reset pair
        }
    }
    for(int i = 0; i < N; i++){
        free(pairs[i]);
    }
    free(pairs);
    return count;
}

/**
 * @brief Helper function for lines
 * Checks if a line is formed by a value in a box
 * I.e. if a value can be set in all cells with the same x or y coordinate, in a box
 * 
 * @param poss Array of possibilities
 * @param boxSize Size of the box
 * @param start_y Start y coordinate of the box
 * @param start_x Start x coordinate of the box
 * @param val Value to check
 * @param line Line struct
*/
void get_line(char poss[N][N][N], int boxSize, int start_y, int start_x, int val, line_t *line){
    int count = 0;
    int first_x = 0;
    int first_y = 0;
    int row_flag = 1;
    int col_flag = 1;
    bool flag_set = false;

    for(int i = 0; i < boxSize; i++){
        for(int j = 0; j < boxSize; j++){
            int x = j + start_x;
            int y = i + start_y;
            if(poss[y][x][val-1] != '0'){ // If cell can set the possible value
                if(count == 0){
                    first_x = x;
                    first_y = y;
                }
                if(first_x != x){
                    col_flag = 0;
                    flag_set = true;
                }
                if(first_y != y){
                    row_flag = 0;
                    flag_set = true;
                }
                count++;
            }
        }
    }
    if(flag_set){
        if(row_flag){
            line->val = val;
            line->x_coord = first_x;
            line->y_coord = first_y;
            line->dir = 0;
        }else if(col_flag){
            line->val = val;
            line->x_coord = first_x;
            line->y_coord = first_y;
            line->dir = 1;
        }        
    }
}

/**
 * @brief Eliminate line
 * If a line is formed by a value in a box, eliminate possibility in other cells in the line
 * 
 * @param poss Array of possibilities
 * @param line Line struct
 * @param start_x Start x coordinate of the box
 * @param start_y Start y coordinate of the box
*/
void eliminate_line(char poss[N][N][N], line_t *line, int start_x, int start_y){
    // dir 0 = row, dir 1 = col
    if(line->dir == 0){
        for(int i = 0; i < N; i++){
            if(start_x <= i && i < (start_x+sqrt_N)) continue;
            poss[line->y_coord][i][line->val-1] = '0';
        }
    }else if(line->dir == 1){
        for(int i = 0; i < N; i++){
            if(start_y <= i && i < (start_y+sqrt_N)) continue;
            poss[i][line->x_coord][line->val-1] = '0';
        }
    }
}

int lines(char poss[N][N][N]){
    int count = 0;
    int boxSize = sqrt_N;
    line_t *line = calloc(1, sizeof(line_t));

    for(int box = 0; box < N; box++){
        int start_x = (box % boxSize) * boxSize;
        int start_y = (box / boxSize) * boxSize;
        for(int val = 1; val < (N+1); val++){
            line->val = 0; // Reset line
            get_line(poss, boxSize, start_y, start_x, val, line);
            if(line->val != 0){
                eliminate_line(poss, line, start_x, start_y);
                count++;
            }
        }
    }
    free(line);
    return count;
}

void copy_board(char OG_board[N][N], char current_board[N][N]){
    for(int j = 0; j < N; j++){
        for(int k = 0; k < N; k++){
            current_board[j][k] = OG_board[j][k];
        }
    }
}

void eliminate_possibilities(char board[N][N], char possibilities[N][N][N]){
    int lucky = 1, easy = 1, pair = 1, previous_pair = 0, line = 1, previous_line = 0;
    while(lucky != 0 || easy != 0 || line != previous_line || pair != previous_pair){ // Loop until no changes
        lucky = the_lucky_one(board, possibilities);    // If cell only has one possibility, set it
        easy = the_easy_one(board, possibilities);      // If only one cell can set a value in a box, set it
        previous_pair = pair;
        pair = pairs(possibilities);                    // If only two cells in a box can set the same two values, eliminate other possibilities in the cells
        previous_line = line;
        line = lines(possibilities);                    // If a line is formed by a value in a box, eliminate value in other cells in the line
    }
}