# Compiler
CC = gcc

# Directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = $(SRC_DIR) # Place object files in src/

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11 -g -I$(INC_DIR)

# Libraries
LIBS = -lrt -lpthread

# Object files (use wildcard to find .c files and generate .o paths)
SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

# Target executable
TARGET = atomsync 

# Build target executable
all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LIBS)

# Compile source files into object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/%.h
	$(CC) $(CFLAGS) -c $< -o $@

# Run tests (Update path if test script moved or depends on new structure)
# test: $(TARGET)
# 	python3 test/parsertests/test.py

# Clean up
clean:
	rm -f $(OBJ) $(TARGET)