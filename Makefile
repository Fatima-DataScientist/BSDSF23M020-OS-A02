CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
SRC = src/lsv1.4.0.c
OBJ = obj/lsv1.4.0.o
BIN = bin/ls

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ)

$(OBJ): $(SRC)
	mkdir -p obj
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ)

clean:
	rm -rf obj/*.o $(BIN)
