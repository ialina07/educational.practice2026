#include "huffmanTree.h"
#include "priorityQueue.h"
#include <stdlib.h>

Node* buildHuffmanTree(int frequencies[256])
{
    PriorityQueue* pq = pqCreate();
    if (!pq) {
        return NULL;
    }

    // Создаём узел для каждого символа с ненулевой частотой
    for (int i = 0; i < 256; i++) {
        if (frequencies[i] > 0) {
            Node* node = (Node*)malloc(sizeof(Node));
            if (!node) {
                pqFree(pq);
                return NULL;
            }
            node->symbol = (unsigned char)i;
            node->freq = frequencies[i];
            node->left = NULL;
            node->right = NULL;
            pqPush(pq, node);
        }
    }

    // Если нет ни одного символа с ненулевой частотой
    if (pqSize(pq) == 0) {
        pqFree(pq);
        return NULL;
    }

    // Сливаем узлы, пока не останется один
    while (pqSize(pq) > 1) {
        Node* left = pqPopMin(pq);
        Node* right = pqPopMin(pq);

        Node* parent = (Node*)malloc(sizeof(Node));
        if (!parent) {
            pqFree(pq);
            return NULL;
        }
        parent->symbol = 0;
        parent->freq = left->freq + right->freq;
        parent->left = left;
        parent->right = right;

        pqPush(pq, parent);
    }

    Node* root = pqPopMin(pq);
    pqFree(pq);
    return root;
}

void freeHuffmanTree(Node* root)
{
    if (!root) {
        return;
    }
    freeHuffmanTree(root->left);
    freeHuffmanTree(root->right);
    free(root);
}

static void generateCodesRecursive(Node* node, uint32_t code, int length,
    uint32_t codes[256], int lengths[256])
{
    if (!node) {
        return;
    }

    // Если лист (нет потомков)
    if (!node->left && !node->right) {
        codes[node->symbol] = code;
        lengths[node->symbol] = length;
        return;
    }

    // Идём влево — добавляем 0
    generateCodesRecursive(node->left, (code << 1) | 0, length + 1, codes, lengths);
    // Идём вправо — добавляем 1
    generateCodesRecursive(node->right, (code << 1) | 1, length + 1, codes, lengths);
}

void generateCodes(Node* root, uint32_t codes[256], int lengths[256])
{
    for (int i = 0; i < 256; i++) {
        codes[i] = 0;
        lengths[i] = 0;
    }

    if (!root) {
        return;
    }

    generateCodesRecursive(root, 0, 0, codes, lengths);
}