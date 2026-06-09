#include <stdio.h>
#include <stdlib.h>
#include "../src/priorityQueue.h"

// Вспомогательная функция для создания узла
static Node* createTestNode(unsigned char symbol, int freq)
{
    Node* node = (Node*)malloc(sizeof(Node));
    if (!node) {
        return NULL;
    }
    node->symbol = symbol;
    node->freq = freq;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Освобождение узлов
static void freeTestNode(Node* node)
{
    if (node) {
        free(node);
    }
}

int main()
{
    printf("\nTesting PriorityQueue\n\n");

    // Тест 1: создание очереди
    printf("Test 1: Create queue\n");
    PriorityQueue* pq = pqCreate();
    if (!pq) {
        printf("FAIL: cannot create queue\n");
        return 1;
    }
    printf("PASS: queue created\n\n");

    // Тест 2: пустая очередь
    printf("Test 2: Empty queue\n");
    if (pqSize(pq) != 0) {
        printf("FAIL: size should be 0, got %d\n", pqSize(pq));
        return 1;
    }
    Node* emptyPop = pqPopMin(pq);
    if (emptyPop != NULL) {
        printf("FAIL: pop from empty should return NULL\n");
        return 1;
    }
    printf("PASS: empty queue works\n\n");

    // Тест 3: добавление одного элемента
    printf("Test 3: Push one element\n");
    Node* n1 = createTestNode('A', 10);
    pqPush(pq, n1);
    if (pqSize(pq) != 1) {
        printf("FAIL: size should be 1, got %d\n", pqSize(pq));
        return 1;
    }
    printf("PASS: size = 1\n\n");

    // Тест 4: извлечение одного элемента
    printf("Test 4: Pop one element\n");
    Node* popped = pqPopMin(pq);
    if (popped != n1) {
        printf("FAIL: popped wrong node\n");
        return 1;
    }
    if (popped->freq != 10) {
        printf("FAIL: wrong frequency, expected 10, got %d\n", popped->freq);
        return 1;
    }
    if (pqSize(pq) != 0) {
        printf("FAIL: size should be 0 after pop, got %d\n", pqSize(pq));
        return 1;
    }
    printf("PASS: pop works\n\n");
    freeTestNode(popped);

    // Тест 5: добавление нескольких элементов с разными частотами
    printf("Test 5: Push multiple elements\n");
    Node* a = createTestNode('A', 5);
    Node* b = createTestNode('B', 2);
    Node* c = createTestNode('C', 8);
    Node* d = createTestNode('D', 1);
    Node* e = createTestNode('E', 3);

    pqPush(pq, a);
    pqPush(pq, b);
    pqPush(pq, c);
    pqPush(pq, d);
    pqPush(pq, e);

    if (pqSize(pq) != 5) {
        printf("FAIL: size should be 5, got %d\n", pqSize(pq));
        return 1;
    }
    printf("PASS: size = 5\n\n");

    // Тест 6: извлечение в правильном порядке (от меньшей частоты к большей)
    printf("Test 6: Pop order (min to max)\n");
    int expected[] = {1, 2, 3, 5, 8};
    for (int i = 0; i < 5; i++) {
        Node* min = pqPopMin(pq);
        if (!min) {
            printf("FAIL: pop returned NULL at step %d\n", i);
            return 1;
        }
        if (min->freq != expected[i]) {
            printf("FAIL: step %d: expected freq=%d, got %d\n",
                   i, expected[i], min->freq);
            return 1;
        }
        printf("  Pop %d: freq=%d (symbol=%c)\n",
               i+1, min->freq, min->symbol);
        freeTestNode(min);
    }
    printf("PASS: correct pop order\n\n");

    // Тест 7: очередь должна быть пустой
    printf("Test 7: Queue empty after all pops\n");
    if (pqSize(pq) != 0) {
        printf("FAIL: size should be 0, got %d\n", pqSize(pq));
        return 1;
    }
    printf("PASS: queue empty\n\n");

    // Тест 8: добавление больше 256 элементов
    printf("Test 8: Dynamic expansion\n");
    PriorityQueue* pqBig = pqCreate();
    Node* manyNodes[300];
    for (int i = 0; i < 300; i++) {
        manyNodes[i] = createTestNode((unsigned char)(i % 256), i);
        pqPush(pqBig, manyNodes[i]);
    }
    if (pqSize(pqBig) != 300) {
        printf("FAIL: size should be 300, got %d\n", pqSize(pqBig));
        return 1;
    }
    // Проверяем, что минимум на месте
    Node* minBig = pqPopMin(pqBig);
    if (minBig->freq != 0) {
        printf("FAIL: min should be 0, got %d\n", minBig->freq);
        return 1;
    }
    freeTestNode(minBig);
    
    while (pqSize(pqBig) > 0) {
        freeTestNode(pqPopMin(pqBig));
    }
    pqFree(pqBig);
    printf("PASS: dynamic expansion works\n\n");

    // Тест 9: освобождение очереди
    printf("Test 9: Free queue\n");
    pqFree(pq);
    printf("PASS: queue freed\n\n");

    printf("ALL TESTS PASSED\n");
    return 0;
}