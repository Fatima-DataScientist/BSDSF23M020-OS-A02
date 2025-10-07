CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
SRC = src/lsv1.3.0.c
OBJ = obj/lsv1.3.0.o
BIN = bin/ls

all: $(BIN)

$(BIN): $(OBJ)
	mkdir -p bin
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)

$(OBJ): $(SRC)
	mkdir -p obj
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)

clean:
	rm -rf obj/*.o $(BIN)

