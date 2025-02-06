#include "common.h"
#include "utils.h"
#include <stdlib.h>

/*****************************************************
 * Unassigned cells are counted and the coordinates 
 * are set in an array. The possibilities array is
 * updated with possible values for each unassigned 
 * cell in the board.
 ****************************************************/
/**
 * @brief Count unassigned cells
 * 
 * @param board Sudoku board
 * @return Number of unassigned cells
*/
int count_unassigned(char board[N][N]);

/**
 * @brief Set unassigned cells
 * Sets the x and y coordinates of unassigned cells and puts them in an array
 *  
 * @param board Sudoku board
 * @param N_unAssign Number of unassigned cells
 * @return Array of unassigned cells
*/
unAssigned_t **set_unassigned(char board[N][N], int N_unAssign);

/**
 * @brief Free memory
 * 
 * @param unAssignInd Array of unassigned cells
 * @param N_unAssign Number of unassigned cells
 * @param possibilities Array of possibilities
*/
void destroy_unassigned(unAssigned_t **unAssignInd, int N_unAssign);

/**
 * @brief Set possibilities
 * Sets the possibilities for each cell in the board
 * 
 * @param board Sudoku board 
 * @param possibilities 3D array of possibilities for each cell
 * EX: possibilities[y][x] = [ 1 0 0 4 5 6 7 0 9 ]
*/
void set_possibilities(char board[N][N], char possibilities[N][N][N]);

/**
 * @brief Update possibilities
 * Updates the possibilities array
 * If a cell is assigned, the possibilities array is set to NULL
 * If a cell is unassigned, the possibilities array is updated with possible values
 * 
 * @param board Sudoku board
 * @param poss Array of possibilities
*/
void new_update_possibilities(char board[N][N], char poss[N][N][N]);
//=====================================================================================
/*****************************************************
 * The logical solver functions uses the possibilities
 * array to solve the sudoku board as much as possible.
 ****************************************************/
/**
 * @brief The lucky one
 * If a cell only has one possibility, set it
 * 
 * @param board Sudoku board
 * @param poss Array of possibilities
 * @return Number of cells set
 * EX: possibilities[0] = [1, 0, 0, 0, 0, 0, 0, 0, 0] 
*/
int the_lucky_one(char board[N][N], char poss[N][N][N]);

/**
 * @brief The easy one
 * If only one cell in a box can set a value, set it
 * 
 * @param board Sudoku board
 * @param poss Array of possibilities
 * @return Number of cells set
*/
int the_easy_one(char board[N][N], char poss[N][N][N]);

/**
 * @brief Search for possible pairs
 * If only two cells in a box can set the same two values, 
 * eliminate other possibilities in the cells
 * 
 * @param poss Array of possibilities
 * @return Number of pairs found
 * EX: possibilities[4][5] = [ 1 4 5 6 7 9 ]
 *     possibilities[5][4] = [ 1 4 5 6 7 9 ]
 *     PAIR FOUND
 *     possibilities[4][5] = [ 6 7 ]
 *     possibilities[5][4] = [ 6 7 ]
*/
int pairs(char poss[N][N][N]);

/**
 * @brief Search for lines
 * If a line is formed by a value in a box, eliminate value in other cells in the line
 * 
 * @param poss Array of possibilities
 * @return Number of lines found
*/
int lines(char poss[N][N][N]);

/**
 * @brief Copy board
 * 
 * @param OG_board Original board
 * @param current_board Current board
*/
void copy_board(char OG_board[N][N], char current_board[N][N]);

/**
 * @brief Eliminate possibilities
 * 
 * @param board Sudoku board
 * @param possibilities Array of possibilities
*/
void eliminate_possibilities(char board[N][N], char possibilities[N][N][N]);