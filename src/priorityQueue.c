#include "priorityQueue.h"
#include <stdlib.h>

PriorityQueue* pqCreate(void)
{
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    if (!pq) {
        return NULL;
    }
    pq->capacity = 256;
    pq->size = 0;
    pq->arr = (Node**)malloc(sizeof(Node*) * pq->capacity);
    if (!pq->arr) {
        free(pq);
        return NULL;
    }
    return pq;
}

static void swap(Node** a, Node** b)
{
    Node* tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heapUp(PriorityQueue* pq, int index)
{
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (pq->arr[parent]->freq <= pq->arr[index]->freq) {
            break;
        }
        swap(&pq->arr[parent], &pq->arr[index]);
        index = parent;
    }
}

static void heapDown(PriorityQueue* pq, int index)
{
    int size = pq->size;
    while (1) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < size && pq->arr[left]->freq < pq->arr[smallest]->freq) {
            smallest = left;
        }
        if (right < size && pq->arr[right]->freq < pq->arr[smallest]->freq) {
            smallest = right;
        }
        if (smallest == index) {
            break;
        }
        swap(&pq->arr[index], &pq->arr[smallest]);
        index = smallest;
    }
}

void pqPush(PriorityQueue* pq, Node* node)
{
    if (!pq || !node) {
        return;
    }

    if (pq->size >= pq->capacity) {
        pq->capacity *= 2;
        Node** newArr = (Node**)realloc(pq->arr, sizeof(Node*) * pq->capacity);
        if (!newArr) {
            return;
        }
        pq->arr = newArr;
    }

    pq->arr[pq->size] = node;
    heapUp(pq, pq->size);
    pq->size++;
}

Node* pqPopMin(PriorityQueue* pq)
{
    if (!pq || pq->size == 0) {
        return NULL;
    }

    Node* min = pq->arr[0];
    pq->size--;
    pq->arr[0] = pq->arr[pq->size];
    heapDown(pq, 0);
    return min;
}

int pqSize(PriorityQueue* pq)
{
    if (!pq) {
        return 0;
    }
    return pq->size;
}

void pqFree(PriorityQueue* pq)
{
    if (!pq) {
        return;
    }
    if (pq->arr) {
        free(pq->arr);
    }
    free(pq);
}