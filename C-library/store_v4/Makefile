CC = gcc
CFLAGS = -Wall -pedantic
LDFLAGS = -lcunit
UTILITIES_DIR = ./utilities/src
BUILD_DIR = ./build

# Rule for compiling store.c into store.o
store.o: store.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)/$@

# Rule for compiling cart.c into cart.o
cart.o: cart.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)/$@

# Rule for compiling unit_tests.c into unit_tests.o
unit_tests.o: unit_tests.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)/$@

# Rule for compiling common.c into common.o
common.o: $(UTILITIES_DIR)/common.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)/$@

# Rule for compiling hash_table.c into hash_table.o
hash_table.o: $(UTILITIES_DIR)/hash_table.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)/$@

# Rule for compiling linked_list.c into linked_list.o
linked_list.o: $(UTILITIES_DIR)/linked_list.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)/$@

# Rule for compiling utils.c into utils.o
utils.o: $(UTILITIES_DIR)/utils.c
	$(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)/$@

# Rule for building valtest
valtest: store.o cart.o unit_tests.o common.o hash_table.o linked_list.o
	$(CC) $(addprefix $(BUILD_DIR)/, $^) -o $(BUILD_DIR)/$@ $(LDFLAGS)
	valgrind --leak-check=full $(BUILD_DIR)/$@

# Rule for building gdbtest
gdbtest: store.o cart.o common.o unit_tests.o hash_table.o linked_list.o
	$(CC) $(addprefix $(BUILD_DIR)/, $^) -o $(BUILD_DIR)/$@ $(LDFLAGS)
	gdb $(BUILD_DIR)/$@

# Rule for building valmenu
valmenu: menu.o utils.o store.o cart.o common.o hash_table.o linked_list.o
	$(CC) $(addprefix $(BUILD_DIR)/, $^) -o $(BUILD_DIR)/$@ $(LDFLAGS)
	valgrind --leak-check=full $(BUILD_DIR)/$@

# Rule for running valtest
runvaltest:
	valgrind --leak-check=full $(BUILD_DIR)/valtest

# Rule for running gdbtest
rungdbtest:
	gdb $(BUILD_DIR)/gdbtest

# Rule for running valmenu
runvalmenu:
	valgrind --leak-check=full $(BUILD_DIR)/valmenu

# Rule for cleaning the build directory
clean:
	rm -f $(BUILD_DIR)/*.o $(BUILD_DIR)/valtest
