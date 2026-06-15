#include "../src/priorityQueue.h"
#include <stdio.h>
#include <stdlib.h>

// Тестовая структура, совместимая по памяти с реальной struct Node
struct TestNode {
    unsigned char symbol;
    int frequency;
    struct TestNode* left;
    struct TestNode* right;
};

// Вспомогательная функция для создания узла
static struct TestNode* createTestNode(unsigned char symbol, int freq)
{
    struct TestNode* node = (struct TestNode*)malloc(sizeof(struct TestNode));
    if (!node) {
        return NULL;
    }
    node->symbol = symbol;
    node->frequency = freq;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Освобождение узлов
static void freeTestNode(struct TestNode* node)
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
    PriorityQueue* priorityQueue = priorityQueueCreate();
    if (!priorityQueue) {
        printf("FAIL: cannot create queue\n");
        return 1;
    }
    printf("PASS: queue created\n\n");

    // Тест 2: пустая очередь
    printf("Test 2: Empty queue\n");
    if (priorityQueueSize(priorityQueue) != 0) {
        printf("FAIL: size should be 0, got %d\n", priorityQueueSize(priorityQueue));
        return 1;
    }
    Node* emptyPop = priorityQueuePopMin(priorityQueue);
    if (emptyPop != NULL) {
        printf("FAIL: pop from empty should return NULL\n");
        return 1;
    }
    printf("PASS: empty queue works\n\n");

    // Тест 3: добавление одного элемента
    printf("Test 3: Push one element\n");
    struct TestNode* n1 = createTestNode('A', 10);
    priorityQueuePush(priorityQueue, (Node*)n1);
    if (priorityQueueSize(priorityQueue) != 1) {
        printf("FAIL: size should be 1, got %d\n", priorityQueueSize(priorityQueue));
        return 1;
    }
    printf("PASS: size = 1\n\n");

    // Тест 4: извлечение одного элемента
    printf("Test 4: Pop one element\n");
    Node* popped = priorityQueuePopMin(priorityQueue);
    if (popped != (Node*)n1) {
        printf("FAIL: popped wrong node\n");
        return 1;
    }
    if (getFrequency(popped) != 10) {
        printf("FAIL: wrong frequency, expected 10, got %d\n", getFrequency(popped));
        return 1;
    }
    if (priorityQueueSize(priorityQueue) != 0) {
        printf("FAIL: size should be 0 after pop, got %d\n", priorityQueueSize(priorityQueue));
        return 1;
    }
    printf("PASS: pop works\n\n");
    freeTestNode(n1);

    // Тест 5: добавление нескольких элементов с разными частотами
    printf("Test 5: Push multiple elements\n");
    struct TestNode* a = createTestNode('A', 5);
    struct TestNode* b = createTestNode('B', 2);
    struct TestNode* c = createTestNode('C', 8);
    struct TestNode* d = createTestNode('D', 1);
    struct TestNode* e = createTestNode('E', 3);

    priorityQueuePush(priorityQueue, (Node*)a);
    priorityQueuePush(priorityQueue, (Node*)b);
    priorityQueuePush(priorityQueue, (Node*)c);
    priorityQueuePush(priorityQueue, (Node*)d);
    priorityQueuePush(priorityQueue, (Node*)e);

    if (priorityQueueSize(priorityQueue) != 5) {
        printf("FAIL: size should be 5, got %d\n", priorityQueueSize(priorityQueue));
        return 1;
    }
    printf("PASS: size = 5\n\n");

    // Тест 6: извлечение в правильном порядке (от меньшей частоты к большей)
    printf("Test 6: Pop order (min to max)\n");
    int expected[] = { 1, 2, 3, 5, 8 };
    for (int i = 0; i < 5; i++) {
        Node* min = priorityQueuePopMin(priorityQueue);
        if (!min) {
            printf("FAIL: pop returned NULL at step %d\n", i);
            return 1;
        }
        if (getFrequency(min) != expected[i]) {
            printf("FAIL: step %d: expected freq=%d, got %d\n",
                i, expected[i], getFrequency(min));
            return 1;
        }
        printf("  Pop %d: freq=%d (symbol=%c)\n",
            i + 1, getFrequency(min), getSymbol(min));
        freeTestNode((struct TestNode*)min);
    }
    printf("PASS: correct pop order\n\n");

    // Тест 7: очередь должна быть пустой
    printf("Test 7: Queue empty after all pops\n");
    if (priorityQueueSize(priorityQueue) != 0) {
        printf("FAIL: size should be 0, got %d\n", priorityQueueSize(priorityQueue));
        return 1;
    }
    printf("PASS: queue empty\n\n");

    // Тест 8: добавление больше 256 элементов
    printf("Test 8: Dynamic expansion\n");
    PriorityQueue* priorityQueueBig = priorityQueueCreate();
    struct TestNode* manyNodes[300];
    for (int i = 0; i < 300; i++) {
        manyNodes[i] = createTestNode((unsigned char)(i % 256), i);
        priorityQueuePush(priorityQueueBig, (Node*)manyNodes[i]);
    }
    if (priorityQueueSize(priorityQueueBig) != 300) {
        printf("FAIL: size should be 300, got %d\n", priorityQueueSize(priorityQueueBig));
        return 1;
    }
    // Проверяем, что минимум на месте
    Node* minBig = priorityQueuePopMin(priorityQueueBig);
    if (getFrequency(minBig) != 0) {
        printf("FAIL: min should be 0, got %d\n", getFrequency(minBig));
        return 1;
    }
    freeTestNode((struct TestNode*)minBig);

    while (priorityQueueSize(priorityQueueBig) > 0) {
        freeTestNode((struct TestNode*)priorityQueuePopMin(priorityQueueBig));
    }
    priorityQueueFree(priorityQueueBig);
    printf("PASS: dynamic expansion works\n\n");

    // Тест 9: освобождение очереди
    printf("Test 9: Free queue\n");
    priorityQueueFree(priorityQueue);
    printf("PASS: queue freed\n\n");

    printf("ALL PQ TESTS PASSED\n");
    return 0;
}