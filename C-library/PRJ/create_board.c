#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <omp.h>
#include "utils.h"
#include "common.h"
#include "logical_solver.h"
#include "set_board.h"

/**
 * @file create_board.c
 * 
 * @brief Creates 100 9x9 Sudoku boards with varying number of empty cells,
 * all of wich have the same solution as the original board.
 * Hardcoded original board (and other values can easily be changed inside the code).
 */

typedef struct cord{
    int x;
    int y;
} cord_t;

/**
 * @brief Generate random pair of x and y coordinates
 * 
 * @param cord Pointer to coordinates
 */
void random_pair(cord_t *cord){
    cord->x = rand() % (9);
    cord->y = rand() % (9);
}

/**
 * @brief Replace cell with 0
 * 
 * @param board Sudoku board
 * @param cord X and Y coordinates
 */
void remove_cell(char board[N][N], cord_t *cord){
    board[cord->y][cord->x] = '0';
}

/**
 * @brief Check if a board remains equal to the original board
 * 
 * @param original_board Original board
 * @param solved_board Solved board
 * @return True if boards remain equal, otherwise false
 */
bool boards_remains_equal(char original_board[N][N], char solved_board[N][N]){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            if(original_board[i][j] != solved_board[i][j]){
                return false;
            }
        }
    }
    return true;
}

/**
 * @brief Replace all cells with 0
 * 
 * @param board Sudoku board
 */
void empty_board(char board[N][N]){
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            board[i][j] = '0';
        }
    }
}

/**
 * @brief Creates an input .txt file with a Sudoku board
 * @note The file path and name is hardcoded
 * 
 * @param board Sudoku board
 * @param file_count Number thats added to the filename
 */
void create_file(char board[N][N], int file_count){
    FILE *f;
    char filename[256];
    sprintf(filename, "input/txt9/9x9board%d.txt", file_count);
    f = fopen(filename, "w");
    if (f == NULL) {
        perror("fopen");
        return;
    }
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            fprintf(f, "%c ", board[i][j]);
        }
        fprintf(f, "\n");
    }
    fclose(f);

    printf("Created file %s\n", filename);
}

/**
 * @brief Modified version of Solve. See main.c for original Solve function
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
 * @brief Modified version of main and Solve. See main.c for original main and Solve functions
 */
bool solve_board(char board[N][N]){
    N = 9;
    sqrt_N = 3;

    bool solution = false;
    char possibilities[N][N][N];
    set_possibilities(board, possibilities);            // Set possibilities
    eliminate_possibilities(board, possibilities);      // Eliminate possibilities and set logical values
    int N_unAssign = count_unassigned(board);

    if(N_unAssign == 0){                                // If no unassigned cells, print solution. Else brute force
        return check_entire_board(board);
    }else{
        unAssigned_t **unAssignInd = set_unassigned(board, N_unAssign); // Set unassigned cells (x, y)
        #pragma omp parallel
        {
            #pragma omp master
            {
                solution = Solve(board, unAssignInd, N_unAssign, &solution, 0, possibilities);
            }
        }
        destroy_unassigned(unAssignInd, N_unAssign);
    }
    return solution;
}

/**
 * @brief Creates a new Sudoku puzzle with as many empty cells as possible,
 * while still having the same solution as the original board.
 * The new board is saved to a .txt file.
 * 
 * @param original_board Original Sudoku board
 * @param file_count Number thats added to the filename
 */
void create_board(char original_board[N][N], int file_count){
    cord_t *cord = malloc(sizeof(cord_t));
    char new_board[N][N];
    copy_board(original_board, new_board);
    // Remove up to 10 random cells (First few cells doesn't affect the solution)
    for(int i = 0; i < 10; i++){ 
        random_pair(cord);
        remove_cell(new_board, cord);
    }

    char test_board[N][N];
    int empty_cells = count_unassigned(new_board);
    bool assigned = true;
    int max = 64;                               // Max number of empty cells (81 - 64 = 17)
    int attempts = 0;

    double start = get_wall_time();             // Start timer
    while(empty_cells < max){
        attempts++;
        if(attempts > 10000){                   // Max number of attempts
            printf("Max number of attempts reached\n");
            break;
        }
                                                
        while(assigned){                        // Loop until a filled cell is found
            random_pair(cord);                  // Create random pair
            if(new_board[cord->y][cord->x] != '0'){
                assigned = false;
            }
        }

        remove_cell(new_board, cord);           // Remove random cell
        copy_board(new_board, test_board);      // Copy new board to test board
        // If board is solvable and remains equal to original board, continue
        if(solve_board(test_board) && boards_remains_equal(original_board, test_board)){
            empty_cells++;
        }else{  // Else, add cell back
            new_board[cord->y][cord->x] = original_board[cord->y][cord->x];
        }
        assigned = true;
    }
    double end = get_wall_time();               // End timer
    create_file(new_board, file_count);         // Create file
    printf("Empty cells: %d\n", empty_cells);   // Print number of empty cells
    printf("Time: %f\n", end-start);            // Print time
    sud_display_board_std(new_board);           // Display board
    free(cord);
}


int main(){
    N = 9;
    sqrt_N = 3;
    char original_board[N][N];
    char *board_str = "951782436834196275276543198748351629369427851512968743485219367127635984693874512";
    
    // Fill original/new-board with board_str
    for(int i = 0; i < N; i++){
        for(int j = 0; j < N; j++){
            original_board[i][j] = board_str[i*N+j];
        }
    }

    srand((unsigned int)time(NULL));        // Seed random number generator
    sud_display_board_std(original_board);  // Display original board
    
    int file_count = 1;
    while(file_count <= 100){
        printf("=====================================\n");
        create_board(original_board, file_count);
        printf("=====================================\n");
        file_count++;
    }
    return 0;   
}