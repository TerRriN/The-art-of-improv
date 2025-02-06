#include "common.h"
#include <stdbool.h>

/**
 * @brief Check if there are duplicate numbers in a row
 * 
 * @param board Sudoku board
 * @param y Row
 * @return True if there are duplicates, otherwise false
*/
bool DuplicateNumbersinRow(char board[N][N], int y, int x);

/**
 * @brief Check if there are duplicate numbers in a column
 * 
 * @param board Sudoku board
 * @param x Column
 * @return True if there are duplicates, otherwise false
*/
bool DuplicateNumbersinCol(char board[N][N], int y, int x);

/**
 * @brief Check if there are duplicate numbers in a box
 * 
 * @param board Sudoku board
 * @param y Row
 * @param x Column
 * @return True if there are duplicates, otherwise false
*/
bool DuplicateNumbersinBox(char board[N][N], int y, int x);

/**
 * @brief Validate board
 *  
 * @param board Sudoku board
 * @param y Row
 * @param x Column
 * @return True if board is valid, otherwise false
*/
bool ValidateBoard(char board[N][N], int y, int x);

/**
 * @brief Check if the entire board is valid
 * 
 * @param board Sudoku board
 * @return True if board is valid, otherwise false
*/
bool check_entire_board(char board[N][N]);

/**
 * @brief Compare to hardcoded solution
 * 
 * @param board Sudoku board
 * @return True if board is equal to hardcoded solution, otherwise false
 */
bool compare_to_solution(char board[N][N]);