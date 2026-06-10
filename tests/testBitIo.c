#include "../src/bitIo.h"
#include <stdint.h>
#include <stdio.h>

int main()
{
    FILE* f = fopen("test.bin", "wb+");
    if (!f) {
        printf("Cannot open test.bin for writing\n");
        return 1;
    }

    BitWriter bw;
    bitWriterInit(&bw, f);
    bitWriterWriteBits(&bw, 0b10101010, 8);
    bitWriterWriteBits(&bw, 0b11110000, 8);
    bitWriterFlush(&bw);
    fclose(f);

    f = fopen("test.bin", "rb");
    if (!f) {
        printf("Cannot open test.bin for reading\n");
        return 1;
    }

    BitReader br;
    bitReaderInit(&br, f);
    uint32_t byte1 = bitReaderReadBits(&br, 8);
    uint32_t byte2 = bitReaderReadBits(&br, 8);
    fclose(f);

    if (byte1 != 0xAA) {
        printf("FAIL: byte1 = %02X, expected AA\n", byte1);
        return 1;
    } else {
        printf("PASS: byte1 = AA\n");
    }

    if (byte2 != 0xF0) {
        printf("FAIL: byte2 = %02X, expected F0\n", byte2);
        return 1;
    } else {
        printf("PASS: byte2 = F0\n");
    }

    printf("ALL BITIO TESTS PASSED\n");
    return 0;
}