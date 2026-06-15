#include "priorityQueue.h"
#include "huffmanTree.h"
#include <stdlib.h>

PriorityQueue* priorityQueueCreate(void)
{
    PriorityQueue* priorityQueue = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    if (!priorityQueue) {
        return NULL;
    }
    priorityQueue->capacity = 256;
    priorityQueue->size = 0;
    priorityQueue->arr = (Node**)malloc(sizeof(Node*) * priorityQueue->capacity);
    if (!priorityQueue->arr) {
        free(priorityQueue);
        return NULL;
    }
    return priorityQueue;
}

static void swap(Node** a, Node** b)
{
    Node* tmp = *a;
    *a = *b;
    *b = tmp;
}

static void heapUp(PriorityQueue* priorityQueue, int index)
{
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (getFrequency(priorityQueue->arr[parent]) <= getFrequency(priorityQueue->arr[index])) {
            break;
        }
        swap(&priorityQueue->arr[parent], &priorityQueue->arr[index]);
        index = parent;
    }
}

static void heapDown(PriorityQueue* priorityQueue, int index)
{
    int size = priorityQueue->size;
    while (1) {
        int left = 2 * index + 1;
        int right = 2 * index + 2;
        int smallest = index;

        if (left < size && getFrequency(priorityQueue->arr[left]) < getFrequency(priorityQueue->arr[smallest])) {
            smallest = left;
        }
        if (right < size && getFrequency(priorityQueue->arr[right]) < getFrequency(priorityQueue->arr[smallest])) {
            smallest = right;
        }
        if (smallest == index) {
            break;
        }
        swap(&priorityQueue->arr[index], &priorityQueue->arr[smallest]);
        index = smallest;
    }
}

void priorityQueuePush(PriorityQueue* priorityQueue, Node* node)
{
    if (!priorityQueue || !node) {
        return;
    }

    if (priorityQueue->size >= priorityQueue->capacity) {
        priorityQueue->capacity *= 2;
        Node** newArr = (Node**)realloc(priorityQueue->arr, sizeof(Node*) * priorityQueue->capacity);
        if (!newArr) {
            return;
        }
        priorityQueue->arr = newArr;
    }

    priorityQueue->arr[priorityQueue->size] = node;
    heapUp(priorityQueue, priorityQueue->size);
    priorityQueue->size++;
}

Node* priorityQueuePopMin(PriorityQueue* priorityQueue)
{
    if (!priorityQueue || priorityQueue->size == 0) {
        return NULL;
    }

    Node* min = priorityQueue->arr[0];
    priorityQueue->size--;
    priorityQueue->arr[0] = priorityQueue->arr[priorityQueue->size];
    heapDown(priorityQueue, 0);
    return min;
}

int priorityQueueSize(PriorityQueue* priorityQueue)
{
    if (!priorityQueue) {
        return 0;
    }
    return priorityQueue->size;
}

void priorityQueueFree(PriorityQueue* priorityQueue)
{
    if (!priorityQueue) {
        return;
    }
    if (priorityQueue->arr) {
        free(priorityQueue->arr);
    }
    free(priorityQueue);
}