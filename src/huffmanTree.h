#pragma once

#include <stddef.h>
#include <stdint.h>

// Узел дерева Хаффмана
typedef struct Node {

    // символ (0-255)
    unsigned char symbol;

    // частота встречаемости
    int freq;

    // левый потомок
    struct Node* left;

    // правый потомок
    struct Node* right;
} Node;

// Строит дерево Хаффмана по массиву частот (256 элементов)
Node* buildHuffmanTree(int frequencies[256]);

// Освобождает память дерева
void freeHuffmanTree(Node* root);

// Генерирует таблицу кодов (массив из 256 записей: код и его длина)
void generateCodes(Node* root, uint32_t codes[256], int lengths[256]);