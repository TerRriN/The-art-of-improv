#include "common.h"
#include <string.h>

/**
 * @brief Set numbers from .ss file to board
 * Helper function to set_numbers
 * 
 * @param board Sudoku board
 * @param file_name File name
*/
void set_numbers(char board[N][N], char *file_name);

/**
 * @brief Set numbers from .txt file to board
 * Helper function to set_numbers
 * 
 * @param board Sudoku board
 * @param file_name File name
*/
void set_numbers_txt(char board[N][N], char *file_name);

/**
 * @brief Set numbers from file to board
 * 
 * @param board Sudoku board
 * @param file_name File name
*/
void set_numbers(char board[N][N], char *file_name);