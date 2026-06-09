#pragma once

#include <stddef.h>
#include "huffmanTree.h"

// Очередь с приоритетами на основе минимальной кучи (min-heap)
typedef struct {

    // массив указателей на узлы
    Node** arr; 
    
    // текущее количество элементов
    int size;        

    // вместимость выделенного массива
    int capacity;    
} PriorityQueue;

// Создаёт новую пустую очередь
PriorityQueue* pqCreate(void);

// Добавляет узел в очередь
void pqPush(PriorityQueue* pq, Node* node);

// Извлекает и возвращает узел с наименьшей частотой (или NULL, если пусто)
Node* pqPopMin(PriorityQueue* pq);

// Возвращает количество узлов в очереди
int pqSize(PriorityQueue* pq);

// Освобождает память очереди (но не сами узлы)
void pqFree(PriorityQueue* pq);