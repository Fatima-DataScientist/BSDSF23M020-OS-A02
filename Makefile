# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Source and object files
SRC = $(SRC_DIR)/lsv1.1.0.c
OBJ = $(OBJ_DIR)/lsv1.1.0.o

# Output binary
TARGET = $(BIN_DIR)/ls

# Default target
all: $(TARGET)

# Rule to build the target
$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Rule to compile object file
$(OBJ): $(SRC)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean object files and binary
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

