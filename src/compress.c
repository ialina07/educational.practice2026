#include "compress.h"
#include "bitIo.h"
#include "huffmanTree.h"
#include <stdlib.h>
#include <string.h>

#define MAGIC_NUMBER 0x4846 // Идентификатор формата: 'H' (0x48) и 'F' (0x46)

// Заголовок сжатого файла
typedef struct {
    uint16_t magic;
    uint8_t version;
    uint64_t originalSize;
    uint32_t frequencies[256];
} __attribute__((packed)) CompressHeader;

static int writeHeader(FILE* file, uint64_t originalSize, int frequencies[256])
{
    CompressHeader header;
    header.magic = MAGIC_NUMBER;
    header.version = 1;
    header.originalSize = originalSize;

    for (int i = 0; i < 256; i++) {
        header.frequencies[i] = frequencies[i];
    }

    size_t written = fwrite(&header, sizeof(header), 1, file);
    if (written != 1) {
        return -1;
    }
    return 0;
}

int compressFile(const char* inputPath, const char* outputPath)
{
    // Открываем входной файл
    FILE* input = fopen(inputPath, "rb");
    if (!input) {
        return -1;
    }

    // Подсчёт частот
    int frequencies[256] = { 0 };
    int c;
    uint64_t fileSize = 0;
    while ((c = fgetc(input)) != EOF) {
        frequencies[c]++;
        fileSize++;
    }

    // Пустой файл
    if (fileSize == 0) {
        fclose(input);
        // Создаём пустой выходной файл
        FILE* output = fopen(outputPath, "wb");
        if (output) {
            fclose(output);
        }
        return 0;
    }

    // Построение дерева Хаффмана
    Node* root = buildHuffmanTree(frequencies);
    if (!root) {
        fclose(input);
        return -1;
    }

    // Генерация таблицы кодов
    uint32_t codes[256];
    int lengths[256];
    generateCodes(root, codes, lengths);

    // Открываем выходной файл и пишем заголовок
    FILE* output = fopen(outputPath, "wb");
    if (!output) {
        freeHuffmanTree(root);
        fclose(input);
        return -1;
    }

    BitWriter bw;
    bitWriterInit(&bw, output);

    // Записываем заголовок
    if (writeHeader(output, fileSize, frequencies) != 0) {
        freeHuffmanTree(root);
        fclose(input);
        fclose(output);
        return -1;
    }

    // Кодирование данных (второй проход)
    rewind(input);
    while ((c = fgetc(input)) != EOF) {
        uint32_t code = codes[c];
        int len = lengths[c];
        if (len > 0) {
            bitWriterWriteBits(&bw, code, len);
        }
        // Если len == 0 не пишем ничего
    }

    // Сбрасываем буфер
    bitWriterFlush(&bw);

    freeHuffmanTree(root);
    fclose(input);
    fclose(output);

    return 0;
}