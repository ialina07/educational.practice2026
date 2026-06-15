#include "compress.h"
#include "decompress.h"
#include <stdio.h>
#include <string.h>

static void printUsage(const char* programName)
{
    printf("Usage:\n");
    printf("  %s -c <input> <output>   compress file\n", programName);
    printf("  %s -x <input> <output>   decompress file\n", programName);
}

int main(int argc, char* argv[])
{
    if (argc != 4) {
        printUsage(argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "-c") == 0) {
        if (compressFile(argv[2], argv[3]) != 0) {
            fprintf(stderr, "Error: compression failed\n");
            return 1;
        }
        printf("Compression successful\n");
    } else if (strcmp(argv[1], "-x") == 0) {
        if (decompressFile(argv[2], argv[3]) != 0) {
            fprintf(stderr, "Error: decompression failed\n");
            return 1;
        }
        printf("Decompression successful\n");
    } else {
        printUsage(argv[0]);
        return 1;
    }

    return 0;
}