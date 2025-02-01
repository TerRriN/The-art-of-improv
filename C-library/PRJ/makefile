CC = gcc
CFLAGS = -Wall -Wextra -g
OFLAGS = -O3 -march=native -fopenmp
LDFLAGS= -lm

all: main

main: 
	$(CC) $(CFLAGS) -o sudoku main.c $(OFLAGS) $(LDFLAGS)

run9:
	./sudoku input/ss/131367198-v4-17-L5.ss 9 4

run16:
	./sudoku input/txt16/1x16.txt 16 4

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
	rm -f sudoku test *.o
