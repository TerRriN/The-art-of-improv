#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Create an array of size N
int *create_array(int N);

// Fetch values from a file and store them into an array
void fetch_values(char *file_name, int *data);

// Check if an array of ints is sorted
bool check_sorting(int *data, int N);

// Check if the input is correct
bool check_input(int argc, char *argv[]);