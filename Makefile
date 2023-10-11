# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Object files
OBJ = parser.o lexer.o symbol_table.o

# Target executable
TARGET = parser

# Build target executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Compile source files into object files
parser.o: parser.c parser.h
	$(CC) $(CFLAGS) -c parser.c

lexer.o: lexer.c lexer.h
	$(CC) $(CFLAGS) -c lexer.c

symbol_table.o: symbol_table.c symbol_table.h
	$(CC) $(CFLAGS) -c symbol_table.c

# Run tests
test: $(TARGET)
	python3 test/parsertests/test.py

# Clean up
clean:
	rm -f $(OBJ) $(TARGET)