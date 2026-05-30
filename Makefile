CC = gcc
CFLAGS = -Wall -Wextra -Werror -O2 -g
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/huffman

# Файлы для форматирования
FORMAT_FILES = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*.h)

format:
	clang-format -i $(FORMAT_FILES)

tidy:
	clang-tidy $(FORMAT_FILES) -- $(CFLAGS) 2>/dev/null || true

$(TARGET): $(OBJECTS)
	mkdir -p $(BIN_DIR)
	$(CC) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run format tidy