#pragma once

#include <stdbool.h>
#include <stdint.h>

#define MAGIC_NUMBER 0x4846 // Идентификатор формата: 'H' (0x48) и 'F' (0x46)
#define HUFFMAN_VERSION 1

// Заголовок сжатого файла (для compress и decompress)
typedef struct {
    uint16_t magic;
    uint8_t version;
    uint64_t originalSize;
    uint32_t frequencies[256];
} __attribute__((packed)) HuffmanHeader;