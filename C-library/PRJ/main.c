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
 * @file main.c
 * @brief Sudoku solver using logical elimination and brute force
 * 
 * @author Oskar Nylin
 * @date 2024-04-04
*/

// Turn on/off board print
#define DEMO 1

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
        destroy_unassigned(unAssignInd, N_unAssign);
    }
    printf("Time: %f\n", get_wall_time() - start);  // End timer and print
    printf("--------------------\n");
    return 0;
}