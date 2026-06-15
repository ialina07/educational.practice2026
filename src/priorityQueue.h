#pragma once

#include "huffmanTree.h"
#include <stddef.h>

// Очередь с приоритетами на основе минимальной кучи
typedef struct {
    // массив указателей на узлы
    Node** arr;

    // текущее количество элементов
    int size;

    // вместимость выделенного массива
    int capacity;
} PriorityQueue;

// Создаёт новую пустую очередь
PriorityQueue* priorityQueueCreate(void);

// Добавляет узел в очередь
void priorityQueuePush(PriorityQueue* priorityQueue, Node* node);

// Извлекает и возвращает узел с наименьшей частотой (или NULL, если пусто)
Node* priorityQueuePopMin(PriorityQueue* priorityQueue);

// Возвращает количество узлов в очереди
int priorityQueueSize(PriorityQueue* priorityQueue);

// Освобождает память очереди (но не сами узлы)
void priorityQueueFree(PriorityQueue* priorityQueue);