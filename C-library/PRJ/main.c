#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <omp.h>

/**
 * @file main.c
 * @brief Sudoku solver using logical elimination and brute force
 * 
 * @author Oskar Nylin
 * @date 2024-04-04
*/


// Turn on/off board print
#define DEMO 0
int N;
int sqrt_N;

typedef struct unAssignInd{
    int x;
    int y;
} unAssigned_t;

typedef struct pair{
    int x1;
    int x2;
    int y1;
    int y2;
    int val;
} pair_t;

typedef struct line{
    int val;
    int x_coord;
    int y_coord;
    int dir;
} line_t;

/**
 * @brief Simple clock function to measure time
 * @return time in seconds
*/
double get_wall_time() {
    struct timespec time;
    clock_gettime(CLOCK_MONOTONIC, &time);
    return (double)time.tv_sec + (double)time.tv_nsec / 1e9;
}

/**
 * @brief Set numbers from .ss file to board
 * Helper function to set_numbers
 * 
 * @param board Sudoku board
 * @param file_name File name
*/
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

/**
 * @brief Set numbers from .txt file to board
 * Helper function to set_numbers
 * 
 * @param board Sudoku board
 * @param file_name File name
*/
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


/**
 * @brief Set numbers from file to board
 * 
 * @param board Sudoku board
 * @param file_name File name
*/
void set_numbers(char board[N][N], char *file_name){
    char *ext = strrchr(file_name, '.');
    if(strcmp(ext, ".ss") == 0){
        set_numbers_ss(board, file_name);
    }else if(strcmp(ext, ".txt") == 0){
        set_numbers_txt(board, file_name);
    }
}

/**
 * @brief Count unassigned cells
 * 
 * @param board Sudoku board
 * @return Number of unassigned cells
*/
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

/**
 * @brief Set unassigned cells
 * Sets the x and y coordinates of unassigned cells and puts them in an array
 *  
 * @param board Sudoku board
 * @param N_unAssign Number of unassigned cells
 * @return Array of unassigned cells
*/
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

/**
 * @brief Free memory
 * 
 * @param unAssignInd Array of unassigned cells
 * @param N_unAssign Number of unassigned cells
 * @param possibilities Array of possibilities
*/
void Destroy(unAssigned_t **unAssignInd, int N_unAssign){
    for(int i = 0; i < N_unAssign; i++){
        free(unAssignInd[i]);
    }
    free(unAssignInd);
}

/**
 * @brief Check if there are duplicate numbers in a row
 * 
 * @param board Sudoku board
 * @param y Row
 * @return True if there are duplicates, otherwise false
*/
bool DuplicateNumbersinRow(char board[N][N], int y, int x){
    char comparator = board[y][x];
    for(int i = 0; i < N; i++){
        if(board[y][i] == comparator && i != x){
            return true;
        }
    }
    return false;
}

/**
 * @brief Check if there are duplicate numbers in a column
 * 
 * @param board Sudoku board
 * @param x Column
 * @return True if there are duplicates, otherwise false
*/
bool DuplicateNumbersinCol(char board[N][N], int y, int x){
    char comparator = board[y][x];
    for(int i = 0; i < N; i++){
        if(board[i][x] == comparator && i != y){
            return true;
        }
    }
    return false;
}

/**
 * @brief Check if there are duplicate numbers in a box
 * 
 * @param board Sudoku board
 * @param y Row
 * @param x Column
 * @return True if there are duplicates, otherwise false
*/

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


/**
 * @brief Validate board
 *  
 * @param board Sudoku board
 * @param y Row
 * @param x Column
 * @return True if board is valid, otherwise false
*/
bool ValidateBoard(char board[N][N], int y, int x){
    if(DuplicateNumbersinBox(board, y, x))   return false; //BOX
    if(DuplicateNumbersinCol(board, y, x))      return false; //COL
    if(DuplicateNumbersinRow(board, y, x))      return false; //ROW
    return true;
}

/**
 * @brief Check if the entire board is valid
 * 
 * @param board Sudoku board
 * @return True if board is valid, otherwise false
*/
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

/**
 * @brief Print board
 * 
 * @param board Sudoku board
*/
void print_board(char board[N][N]){
    int x = 0;
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            x = board[i][j] - '0';
            printf("%d ", x);
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * @brief Set possibilities
 * Sets the possibilities for each cell in the board
 * 
 * @param board Sudoku board 
 * @param possibilities 3D array of possibilities for each cell
 * EX: possibilities[y][x] = [ 1 0 0 4 5 6 7 0 9 ]
*/
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
/**
 * @brief Update possibilities
 * Updates the possibilities array
 * If a cell is assigned, the possibilities array is set to NULL
 * If a cell is unassigned, the possibilities array is updated with possible values
 * 
 * @param board Sudoku board
 * @param poss Array of possibilities
*/
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

/**
 * @brief Print all possibilities
 *  
 * @param poss Array of possibilities
 * @param N_elements Number of elements in the possibilities array
*/
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

/**
 * @brief Print possibilities for a specific cell
 * 
 * @param poss Array of possibilities
 * @param y Row
 * @param x Column
*/
void print_precise_possibilities(char poss[N][N][N], int y, int x){
    int x_val = 0;
    printf("x: %d, y: %d: [ ", x, y);
    for(int k = 0; k < N; k++){
        x_val = poss[y][x][k] - '0';
        if(x_val != 0) printf("%d ", x_val);
    }
    printf("]\n");
}

/**
 * @brief The lucky one
 * If a cell only has one possibility, set it
 * 
 * @param board Sudoku board
 * @param poss Array of possibilities
 * @return Number of cells set
 * EX: possibilities[0] = [1, 0, 0, 0, 0, 0, 0, 0, 0] 
*/
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

/**
 * @brief The easy one
 * If only one cell in a box can set a value, set it
 * 
 * @param board Sudoku board
 * @param poss Array of possibilities
 * @return Number of cells set
*/
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

/**
 * @brief Search for possible pairs
 * If only two cells in a box can set the same two values, eliminate other possibilities in the cells
 * 
 * @param poss Array of possibilities
 * @return Number of pairs found
 * EX: possibilities[4][5] = [ 1 4 5 6 7 9 ]
 *     possibilities[5][4] = [ 1 4 5 6 7 9 ]
 *     PAIR FOUND
 *     possibilities[4][5] = [ 6 7 ]
 *     possibilities[5][4] = [ 6 7 ]
*/
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


/**
 * @brief Search for lines
 * If a line is formed by a value in a box, eliminate value in other cells in the line
 * 
 * @param poss Array of possibilities
 * @return Number of lines found
*/
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


/**
 * @brief Copy board
 * 
 * @param OG_board Original board
 * @param current_board Current board
*/
void copy_board(char OG_board[N][N], char current_board[N][N]){
    for(int j = 0; j < N; j++){
        for(int k = 0; k < N; k++){
            current_board[j][k] = OG_board[j][k];
        }
    }
}

/**
 * @brief Eliminate possibilities
 * 
 * @param board Sudoku board
 * @param possibilities Array of possibilities
*/
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

/**
 * @brief Brute force algorithm that tries to solve the sudoku board
 * 
 * @param board Sudoku board
 * @param unAssignInd Array of unassigned cells
 * @param N_unAssign Number of unassigned cells
 * @param solutionFound True if solution is found, otherwise false
 * @param depth Depth of the recursion
 * @param possibilities Array of possibilities
 * @return True if solution is found, otherwise false
 * @note Since this function is implemented with OpenMP, will the final return always be false
 *       The solution is found when a thread sets the solutionFound variable to true
 *       This is a workaround to stop all threads
 * 
 * This code was adapted from the following question on stackoverflow:
 * https://stackoverflow.com/questions/68320530/openmp-sudoku-solver-parallelize-algorithm-with-openmp-and-or-mpi
 * and geeksforgeeks:
 * https://www.geeksforgeeks.org/sudoku-backtracking-7/
*/
bool Solve(char board[N][N], unAssigned_t **unAssignInd, int N_unAssign, bool *solutionFound, int depth, char poss[N][N][N]){
    if(N_unAssign == 0){
        return true;
    }
    int tmp = N_unAssign - 1;
    int x = unAssignInd[tmp]->x;
    int y = unAssignInd[tmp]->y;

    #pragma omp taskgroup
    {
        for(int i = 0; i < N; i++){
            if(*solutionFound) break;
            if(poss[y][x][i] == '0') continue;

            board[y][x] = (i+1) + '0'; // Set guess
            if(ValidateBoard(board, y, x)){
                bool local_solution = false;
                #pragma omp task firstprivate(board, N_unAssign) final(depth > 1)
                {
                    char board_copy[N][N];
                    copy_board(board, board_copy);
                    board_copy[y][x] = (i+1) + '0';
                    local_solution = Solve(board_copy, unAssignInd, tmp, solutionFound, depth+1, poss);
                    if(local_solution){
                        if(check_entire_board(board_copy)){
                            printf("SOLUTION\n");
                        }
                        #if DEMO == 1
                        print_board(board_copy);
                        #endif
                        *solutionFound = true; // Workaround to stop all threads
                    }
                }
            }
        }
        #pragma omp taskwait
    }
    board[y][x] = '0'; // No solution, reset value in backtracking
    return false;
}

/**
 * @brief Main function
 * 
 * @param argc Number of arguments
 * @param argv Arguments
 * @return 0 if successful, otherwise 1
*/
int main(int argc, char *argv[]){
    if(argc != 4){
        printf("Usage: %s <file> <N> <N_threads>\n", argv[0]);
        return 1;
    }
    int N_threads = atoi(argv[3]);
    N = atoi(argv[2]);
    sqrt_N = sqrt(N);
    char board[N][N];
    set_numbers(board, argv[1]);                    // Set numbers from file

    printf("--------------------\n");
    printf("Running %s\n", argv[1]);
    
    #if DEMO == 1
    printf("NOF Unassigned cells before elimination: %d\n", count_unassigned(board));
    print_board(board);
    #endif
    bool solution = false;
    double start = get_wall_time();                     // Start timer
    char possibilities[N][N][N];
    set_possibilities(board, possibilities);            // Set possibilities
    eliminate_possibilities(board, possibilities);      // Eliminate possibilities and set logical values
    int N_unAssign = count_unassigned(board);

    #if DEMO == 1
    printf("NOF Unassigned cells after elimination: %d\n", N_unAssign);
    #endif
    if(N_unAssign == 0){                                // If no unassigned cells, print solution. Else brute force
        if(check_entire_board(board)){
            printf("EARLY SOLUTION\n");
        }else{
            printf("NO SOLUTION\n");
        }
        #if DEMO == 1
        print_board(board);
        #endif
    }else{
        unAssigned_t **unAssignInd = set_unassigned(board, N_unAssign); // Set unassigned cells (x, y)
        omp_set_num_threads(N_threads);
        #pragma omp parallel
        {
            #pragma omp master
            {
                Solve(board, unAssignInd, N_unAssign, &solution, 0, possibilities);
                if(!solution){
                    printf("NO SOLUTION\n");
                }
            }
        }
        Destroy(unAssignInd, N_unAssign);
    }
    printf("Time: %f\n", get_wall_time() - start);  // End timer and print
    printf("--------------------\n");
    return 0;
}