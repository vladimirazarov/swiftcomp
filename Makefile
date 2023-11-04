# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Object files
OBJ = parser.o lexer.o symbol_table.o expressions.o ast_tree.o error.o semantic.o sym_table_stack.o

# Target executable
TARGET = semantic 

# Build target executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Compile source files into object files
semantic.o: semantic.c semantic.h
	$(CC) $(CFLAGS) -c semantic.c

parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

lexer.o: lexer.c lexer.h
	$(CC) $(CFLAGS) -c lexer.c

symbol_table.o: symbol_table.c symbol_table.h
	$(CC) $(CFLAGS) -c symbol_table.c

sym_table_stack.o: sym_table_stack.c symbol_table.h
	$(CC) $(CFLAGS) -c sym_table_stack.c

ast_tree.o: ast_tree.c ast_tree.h
	$(CC) $(CFLAGS) -c ast_tree.c

expressions.o: expressions.c expressions.h
	$(CC) $(CFLAGS) -c expressions.c

error.o: error.c error.h
	$(CC) $(CFLAGS) -c error.c

# Run tests
test: $(TARGET)
	python3 test/parsertests/test.py

# Clean up
clean:
	rm -f $(OBJ) $(TARGET)