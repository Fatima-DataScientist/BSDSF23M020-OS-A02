
# =========================================================
# Makefile for lsv1.1.0 (Feature 2 - Long Listing)
# Author: Fatima Tu Zahra
# Description: Builds the ls utility (normal + -l long listing)
# =========================================================

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Source and object files
SRC = $(SRC_DIR)/lsv1.1.0.c
OBJ = $(OBJ_DIR)/lsv1.1.0.o

# Output binary
TARGET = $(BIN_DIR)/lsv1.1.0

# =========================================================
# Default target: build everything
# =========================================================
all: $(TARGET)

# Build rule
$(TARGET): $(OBJ)
	@echo "🔧 Linking object files to create executable..."
	$(CC) $(CFLAGS) $^ -o $@
	@echo "✅ Build complete! Executable created at: $(TARGET)"

# Compile rule for object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "🧱 Compiling source file: $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Run target
run: all
	@echo "🚀 Running program..."
	@$(TARGET)

# Clean rule - remove compiled files
clean:
	@echo "🧹 Cleaning project files..."
	rm -f $(OBJ_DIR)/*.o $(TARGET)

# Very clean rule - removes even directories (optional)
distclean: clean
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# =========================================================
# End of Makefile
# =========================================================

