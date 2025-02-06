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
 * @brief Creates 100 NxN Sudoku boards with varying number of empty cells,
 * all of wich have the same solution as the original board.
 * Hardcoded original board (and other values can easily be changed inside the code).
 */

typedef struct cord{
    int x;
    int y;
} cord_t;

void insert_input(char original_board[N][N], const char *board_str){
    char *token;
    char *str = strdup(board_str); // Duplicate input to avoid modifying original
    int value, i = 0, j = 0;

    token = strtok(str, ",");
    while (token != NULL) {
        value = atoi(token);                // Convert string to integer
        original_board[i][j] = value + '0'; // Convert integer to char
        j++;
        if(j == N){
            j = 0;
            i++;
        }
        token = strtok(NULL, ",");
    }
    free(str);
}

/**
 * @brief Generate random pair of x and y coordinates
 * 
 * @param cord Pointer to coordinates
 */
void random_pair(cord_t *cord){
    cord->x = rand() % (N);
    cord->y = rand() % (N);
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
    sprintf(filename, "input/txt%d/%dx%dboard%d.txt", N, N, N, file_count);
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
    bool solution = false;
    char possibilities[N][N][N];
    set_possibilities(board, possibilities);            // Set possibilities
    eliminate_possibilities(board, possibilities);      // Eliminate possibilities and set logical values
    int N_unAssign = count_unassigned(board);

    if(N_unAssign == 0){                                // If no unassigned cells, print solution. Else brute force
        return check_entire_board(board);
    }else{
        unAssigned_t **unAssignInd = set_unassigned(board, N_unAssign); // Set unassigned cells (x, y)
        omp_set_num_threads(10);
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
    int max = 150;                               // Max number of empty cells
    int attempts = 0;
    double start = get_wall_time();             // Start timer

    while(empty_cells < max){
        attempts++;
        if(attempts > 10000){                   // Max number of attempts
            printf("Max number of attempts reached\n");
            break;
        }

        if(get_wall_time() - start > 25){       // Max time
            printf("Max time reached\n");
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
    //sud_display_board_std(new_board);           // Display board
    print_board(new_board);                     // Print board1
    free(cord);
}


int main(){
    N = 16;
    sqrt_N = 4;
    char original_board[N][N];
    //const char *board_str = "951782436834196275276543198748351629369427851512968743485219367127635984693874512";
    const char *board_str2 =  "9,11,13,5,12,10,16,1,14,15,2,4,6,8,7,3,"
                        "4,15,6,10,13,14,7,3,11,9,8,16,12,2,1,5,"
                        "16,12,3,8,11,2,15,9,5,6,1,7,14,4,13,10,"
                        "14,7,1,2,6,4,8,5,12,10,3,13,15,11,9,16,"
                        "3,6,16,11,5,1,14,8,4,2,15,9,7,13,10,12,"
                        "7,2,10,9,3,15,13,6,8,11,12,14,4,16,5,1,"
                        "13,14,12,15,10,16,11,4,7,3,5,1,8,6,2,9,"
                        "8,4,5,1,2,12,9,7,6,16,13,10,3,15,14,11,"
                        "2,5,8,12,14,13,1,16,3,7,11,15,9,10,6,4,"
                        "15,10,7,4,8,11,12,2,1,14,9,6,5,3,16,13,"
                        "6,9,14,13,7,3,4,10,2,12,16,5,11,1,15,8,"
                        "1,16,11,3,9,5,6,15,13,4,10,8,2,14,12,7,"
                        "10,1,9,7,4,8,2,12,15,13,14,11,16,5,3,6,"
                        "5,13,15,6,16,9,3,14,10,8,4,12,1,7,11,2,"
                        "12,8,2,14,1,6,10,11,16,5,7,3,13,9,4,15,"
                        "11,3,4,16,15,7,5,13,9,1,6,2,10,12,8,14";

    insert_input(original_board, board_str2);   // Insert board string to board
    srand((unsigned int)time(NULL));            // Seed random number generator
    print_board(original_board);                // Print board
    int file_count = 5;
    while(file_count <= 10){
        printf("=====================================\n");
        create_board(original_board, file_count);
        printf("=====================================\n");
        file_count++;
    }
    return 0;   
}