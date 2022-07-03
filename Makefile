CC := gcc
CFLAGS := -g

SRC_DIR   := src
OBJ_DIR   := build/obj
BIN_DIR   := build/bin
BUILD_DIR := build

OUTPUT = $(BIN_DIR)/emulator

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJ := $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY = build clean



build: $(OUTPUT)

clean:
	rm -rf $(BUILD_DIR)

$(OUTPUT): ${OBJ}
	mkdir -p $(@D)
	$(CC) ${OBJ} -o $(OUTPUT)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<