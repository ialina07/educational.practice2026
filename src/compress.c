#include "compress.h"
#include "bitIo.h"
#include "huffman.h"
#include "huffmanTree.h"
#include <stdlib.h>
#include <string.h>

static int writeHeader(FILE* file, uint64_t originalSize, int frequencies[256])
{
    HuffmanHeader header;
    header.magic = MAGIC_NUMBER;
    header.version = HUFFMAN_VERSION;
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

static uint64_t countFrequencies(FILE* input, int frequencies[256])
{
    uint64_t fileSize = 0;
    int c;
    while ((c = fgetc(input)) != EOF) {
        frequencies[c]++;
        fileSize++;
    }
    return fileSize;
}

int compressFile(const char* inputPath, const char* outputPath)
{
    FILE* input = fopen(inputPath, "rb");
    if (!input) {
        return -1;
    }

    int frequencies[256] = { 0 };
    uint64_t fileSize = countFrequencies(input, frequencies);

    if (fileSize == 0) {
        fclose(input);
        FILE* output = fopen(outputPath, "wb");
        if (output) {
            fclose(output);
        }
        return 0;
    }

    Node* root = buildHuffmanTree(frequencies);
    if (!root) {
        fclose(input);
        return -1;
    }

    uint32_t codes[256];
    int lengths[256];
    generateCodes(root, codes, lengths);

    FILE* output = fopen(outputPath, "wb");
    if (!output) {
        freeHuffmanTree(root);
        fclose(input);
        return -1;
    }

    BitWriter bw;
    bitWriterInit(&bw, output);

    if (writeHeader(output, fileSize, frequencies) != 0) {
        freeHuffmanTree(root);
        fclose(input);
        fclose(output);
        return -1;
    }

    rewind(input);
    int c;
    while ((c = fgetc(input)) != EOF) {
        uint32_t code = codes[c];
        int len = lengths[c];
        if (len > 0) {
            bitWriterWriteBits(&bw, code, len);
        }
    }

    bitWriterFlush(&bw);

    freeHuffmanTree(root);
    fclose(input);
    fclose(output);

    return 0;
}