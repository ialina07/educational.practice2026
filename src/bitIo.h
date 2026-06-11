#pragma once

#include <stdint.h>
#include <stdio.h>

typedef struct {
    FILE* file;
    unsigned char buffer;
    int bitsInBuffer;
} BitWriter;

typedef struct {
    FILE* file;
    unsigned char buffer;
    int bitsInBuffer;
} BitReader;

// Инициализирует BitWriter для записи в файл
void bitWriterInit(BitWriter* bw, FILE* file);

// Записывает один бит (0 или 1)
void bitWriterWriteBit(BitWriter* bw, int bit);

// Записывает целое число, используя numBits младших битов
void bitWriterWriteBits(BitWriter* bw, uint32_t bits, int numBits);

// Дописывает оставшиеся биты нулями и сбрасывает буфер в файл
void bitWriterFlush(BitWriter* bw);

// Инициализирует BitReader для чтения из файла
void bitReaderInit(BitReader* br, FILE* file);

// Читает один бит, возвращает 0 или 1, или -1 при ошибке/конце файла
int bitReaderReadBit(BitReader* br);

// Читает целое число из numBits битов
uint32_t bitReaderReadBits(BitReader* br, int numBits);

// Проверяет, достигнут ли конец файла (с учётом буфера)
int bitReaderEof(BitReader* br);
