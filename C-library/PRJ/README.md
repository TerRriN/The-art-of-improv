# Giant Sudoku Solver
This Sudoku solver can solve puzzles of various **SYMETRICAL** sizes. It uses logic based possibility elimination and backtracking bruteforce to solve the puzzle. The solver is able to solve puzzles as large as your hardware allows (stack overflow is a real issue).

__Note:__ The solution time varies heavily on the input puzzle. The solver is able to solve most puzzles in a reasonable time frame. However, some puzzles may take a long time to solve. It all depends on how many cells the solver needs to bruteforce and how many possibilities are available for each cell.

## Usage
The solver is written in C. A makefile is provided to compile the solver. The solver can run with the following commands:  
To compile the solver:
```bash
make
```
To run performance tests on several puzzles:  
The number of threads have to be manually set in the .sh files. Preset to 4. (Change the "N_threads" variable)  
__Note:__ A lot of print statements can be turned on/off in the main.c file, by changing the "DEMO" variable on line 10.  
9x9
```bash
make time9
```
16x16
```bash
make time16
```
25x25
```bash
make time25
```

To run the solver on a specific puzzle, see run commands in the makefile for examples.

## Input
Input files can be in the format of .txt or .ss. If you whish to use your own input data, see input files for examples. Make sure the puzzle is symetrical and the puzzle only has one solution. Easiest way to create a puzzle is to copy one from the web, write a translator that converts the puzzle to the correct format, and save it as a .txt file. The .ss format is used when generating puzzles on this very helpful site [website](https://kjell.haxx.se/sudoku/).
Other sites which I used to generate puzzles:
[website](http://www.sudoku-download.net/sudoku_64x64.php)
[website](https://sudokugeant.cabanova.com/sudoku.html)
[website](https://sudokugeant.cabanova.com/)

## Output
The solver will output one of three statements:
- EARLY SOLUTION __- If the puzzle was solved before the bruteforce was needed__
- SOLUTION __- If the puzzle was solved__
- NO SOLUTION __- If the puzzle was not solved__

The solver will print the solved puzzle to the terminal if the "DEMO" variable is set to 1. The solver will also print the time it took to solve the puzzle.
