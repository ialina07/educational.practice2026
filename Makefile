CC = gcc
CFLAGS = -Wall -Wextra -Werror -O2 -g
SRC_DIR = src
TEST_DIR = tests
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

check-format:
	clang-format --dry-run --Werror $(FORMAT_FILES)

# Тесты
testBitIo: $(OBJ_DIR)/bitIo.o
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -I $(SRC_DIR) $(TEST_DIR)/testBitIo.c $(OBJ_DIR)/bitIo.o -o $(BIN_DIR)/testBitIo
	$(BIN_DIR)/testBitIo

testPq: $(OBJ_DIR)/priorityQueue.o
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -I $(SRC_DIR) $(TEST_DIR)/testPq.c $(OBJ_DIR)/priorityQueue.o -o $(BIN_DIR)/testPq
	$(BIN_DIR)/testPq

testTree: $(OBJ_DIR)/huffmanTree.o $(OBJ_DIR)/priorityQueue.o
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -I $(SRC_DIR) $(TEST_DIR)/testTree.c $(OBJ_DIR)/huffmanTree.o $(OBJ_DIR)/priorityQueue.o -o $(BIN_DIR)/testTree
	$(BIN_DIR)/testTree

testCompress: $(OBJ_DIR)/compress.o $(OBJ_DIR)/bitIo.o $(OBJ_DIR)/huffmanTree.o $(OBJ_DIR)/priorityQueue.o
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -I $(SRC_DIR) $(TEST_DIR)/testCompress.c $(OBJ_DIR)/compress.o $(OBJ_DIR)/bitIo.o $(OBJ_DIR)/huffmanTree.o $(OBJ_DIR)/priorityQueue.o -o $(BIN_DIR)/testCompress
	$(BIN_DIR)/testCompress

testAll: testBitIo testPq testTree testCompress
	@echo "All tests passed!"
	
# Проверка на утечки памяти
valgrindTree: testTree
	valgrind --leak-check=full --show-leak-kinds=all ./$(BIN_DIR)/testTree

valgrindPq: testPq
	valgrind --leak-check=full --show-leak-kinds=all ./$(BIN_DIR)/testPq

valgrindBitIo: testBitIo
	valgrind --leak-check=full --show-leak-kinds=all ./$(BIN_DIR)/testBitIo

.PHONY: clean run format tidy testBitIo testPq testAll