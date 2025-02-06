#ifndef COMMON_H
#define COMMON_H

extern int N;
extern int sqrt_N;

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
#endif

#include <stdio.h>
#include <time.h>

/**
 * @brief Simple clock function to measure time
 * @return time in seconds
*/
double get_wall_time();

/**
 * @brief Print board
 * 
 * @param board Sudoku board
*/
void print_board(char board[N][N]);

/**
 * @brief Print all possibilities
 *  
 * @param poss Array of possibilities
 * @param N_elements Number of elements in the possibilities array
*/
void print_possibilities(char poss[N][N][N]);

/**
 * @brief Print possibilities for a specific cell
 * 
 * @param poss Array of possibilities
 * @param y Row
 * @param x Column
*/
void print_precise_possibilities(char poss[N][N][N], int y, int x);

/**
 * @brief Display standard 9x9 sudoku board in a more readable format
 * 
 * @param board Sudoku board
 */
void sud_display_board_std(char board[N][N]);

/**
 * @brief Display giant sudoku board in a more readable format
 * 
 * @param board Sudoku board
 */
void sud_display_board_giant(char board[N][N]);