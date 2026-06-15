#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Узел дерева Хаффмана
typedef struct Node Node;

// Строит дерево Хаффмана по массиву частот (256 элементов)
Node* buildHuffmanTree(int frequencies[256]);

// Освобождает память дерева
void freeHuffmanTree(Node* root);

// Генерирует таблицу кодов (массив из 256 записей: код и его длина)
void generateCodes(Node* root, uint32_t codes[256], int lengths[256]);

// Функции для доступа к полям узла:

// Проверяет, является ли узел листом
bool isLeaf(Node* node);

// Возвращает символ, хранящийся в узле
unsigned char getSymbol(Node* node);

// Возвращает частоту символа в узле
int getFrequency(Node* node);

// Возвращает левого потомка узла
Node* getLeft(Node* node);

// Возвращает правого потомка узла
Node* getRight(Node* node);