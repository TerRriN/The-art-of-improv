CC = gcc
CFLAGS = -Wall -Wextra -g
OFLAGS = -O3 -march=native -fopenmp
LDFLAGS= -lm

all: main

main: common.o utils.o logical_solver.o main.c set_board.o
	$(CC) $(CFLAGS) $(OFLAGS) -o sudoku main.c common.o utils.o logical_solver.o set_board.o $(LDFLAGS)

create_board: create_board.c common.o utils.o logical_solver.o set_board.o
	$(CC) $(CFLAGS) $(OFLAGS) -o create_board create_board.c common.o utils.o logical_solver.o set_board.o $(LDFLAGS)

function_tester: function_tester.c common.o utils.o logical_solver.o set_board.o
	$(CC) $(CFLAGS) $(OFLAGS) -o function_tester function_tester.c common.o utils.o logical_solver.o set_board.o $(LDFLAGS)

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) $(OFLAGS) -c utils.c

logical_solver.o: logical_solver.c logical_solver.h
	$(CC) $(CFLAGS) $(OFLAGS) -c logical_solver.c

set_board.o: set_board.c set_board.h
	$(CC) $(CFLAGS) $(OFLAGS) -c set_board.c

common.o: common.c common.h
	$(CC) $(CFLAGS) $(OFLAGS) -c common.c


run9:
	./sudoku input/txt9/9x9board1.txt 9 4

run16:
	./sudoku input/txt16/2x16.txt 16 10

run25:
	./sudoku input/txt25/13x25.txt 25 4

time9:
	./run-sudokux9.sh

time16:
	./run-sudokux16.sh

time25:
	./run-sudokux25.sh

gdb:
	gdb ./sudoku input/txt/1x25.txt 25 4
	
run_test: test
	valgrind --leak-check=full ./test

clean:
	rm -f sudoku test *.o create_board function_tester
