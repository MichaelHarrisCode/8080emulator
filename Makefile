CC := gcc
CFLAGS := -g

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

OUTPUT = $(BIN_DIR)/emulator

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY = build clean run



build: ${OBJ}
	$(CC) ${OBJ} -o $(OUTPUT)

clean:
	rm $(OBJ_DIR)/*.o $(OUTPUT)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<