#include "decompress.h"
#include "bitIo.h"
#include "huffman.h"
#include "huffmanTree.h"
#include <stdlib.h>
#include <string.h>

static int validateHeader(HuffmanHeader* header)
{
    if (header->magic != MAGIC_NUMBER) {
        return -1;
    }
    if (header->version != HUFFMAN_VERSION) {
        return -1;
    }
    return 0;
}

int decompressFile(const char* inputPath, const char* outputPath)
{
    FILE* input = fopen(inputPath, "rb");
    if (!input) {
        return -1;
    }

    fseek(input, 0, SEEK_END);
    long fileSize = ftell(input);
    rewind(input);

    if (fileSize == 0) {
        fclose(input);
        FILE* output = fopen(outputPath, "wb");
        if (output) {
            fclose(output);
        }
        return 0;
    }

    HuffmanHeader header;
    size_t read = fread(&header, sizeof(header), 1, input);
    if (read != 1) {
        fclose(input);
        return -1;
    }

    if (validateHeader(&header) != 0) {
        fclose(input);
        return -1;
    }

    if (header.originalSize == 0) {
        fclose(input);
        FILE* output = fopen(outputPath, "wb");
        if (output) {
            fclose(output);
        }
        return 0;
    }

    int frequencies[256];
    for (int i = 0; i < 256; i++) {
        frequencies[i] = header.frequencies[i];
    }

    Node* root = buildHuffmanTree(frequencies);
    if (!root) {
        fclose(input);
        return -1;
    }

    FILE* output = fopen(outputPath, "wb");
    if (!output) {
        freeHuffmanTree(root);
        fclose(input);
        return -1;
    }

    BitReader br;
    bitReaderInit(&br, input);

    uint64_t bytesWritten = 0;

    if (isLeaf(root)) {
        unsigned char symbol = getSymbol(root);
        for (uint64_t i = 0; i < header.originalSize; i++) {
            fputc(symbol, output);
        }
        bytesWritten = header.originalSize;
    } else {
        while (bytesWritten < header.originalSize) {
            Node* current = root;
            while (!isLeaf(current)) {
                int bit = bitReaderReadBit(&br);
                if (bit == -1) {
                    freeHuffmanTree(root);
                    fclose(input);
                    fclose(output);
                    return -1;
                }
                if (bit == 0) {
                    current = getLeft(current);
                } else {
                    current = getRight(current);
                }
            }
            fputc(getSymbol(current), output);
            bytesWritten++;
        }
    }

    freeHuffmanTree(root);
    fclose(input);
    fclose(output);

    return 0;
}