#include "huffmanTree.h"
#include "priorityQueue.h"
#include <stdbool.h>
#include <stdlib.h>

struct Node {
    unsigned char symbol;
    int frequency;
    struct Node* left;
    struct Node* right;
};

bool isLeaf(Node* node)
{
    return node != NULL && node->left == NULL && node->right == NULL;
}

unsigned char getSymbol(Node* node)
{
    return node->symbol;
}

int getFrequency(Node* node)
{
    return node->frequency;
}

Node* getLeft(Node* node)
{
    return node->left;
}

Node* getRight(Node* node)
{
    return node->right;
}

static Node* createNode(unsigned char symbol, int frequency)
{
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) {
        return NULL;
    }
    node->symbol = symbol;
    node->frequency = frequency;
    node->left = NULL;
    node->right = NULL;
    return node;
}

Node* buildHuffmanTree(int frequencies[256])
{
    PriorityQueue* priorityQueue = priorityQueueCreate();
    if (!priorityQueue) {
        return NULL;
    }

    for (int i = 0; i < 256; i++) {
        if (frequencies[i] > 0) {
            Node* node = createNode((unsigned char)i, frequencies[i]);
            if (!node) {
                priorityQueueFree(priorityQueue);
                return NULL;
            }
            priorityQueuePush(priorityQueue, node);
        }
    }

    if (priorityQueueSize(priorityQueue) == 0) {
        priorityQueueFree(priorityQueue);
        return NULL;
    }

    while (priorityQueueSize(priorityQueue) > 1) {
        Node* left = priorityQueuePopMin(priorityQueue);
        Node* right = priorityQueuePopMin(priorityQueue);

        Node* parent = createNode(0, getFrequency(left) + getFrequency(right));
        if (!parent) {
            priorityQueueFree(priorityQueue);
            return NULL;
        }
        parent->left = left;
        parent->right = right;

        priorityQueuePush(priorityQueue, parent);
    }

    Node* root = priorityQueuePopMin(priorityQueue);
    priorityQueueFree(priorityQueue);
    return root;
}

void freeHuffmanTree(Node* root)
{
    if (!root) {
        return;
    }
    freeHuffmanTree(getLeft(root));
    freeHuffmanTree(getRight(root));
    free(root);
}

static void generateCodesRecursive(Node* node, uint32_t code, int length,
    uint32_t codes[256], int lengths[256])
{
    if (!node) {
        return;
    }

    if (isLeaf(node)) {
        codes[getSymbol(node)] = code;
        lengths[getSymbol(node)] = length;
        return;
    }

    generateCodesRecursive(getLeft(node), (code << 1) | 0, length + 1, codes, lengths);
    generateCodesRecursive(getRight(node), (code << 1) | 1, length + 1, codes, lengths);
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