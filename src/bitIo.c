#include "bitIo.h"

void bitWriterInit(BitWriter* bw, FILE* file)
{
    bw->file = file;
    bw->buffer = 0;
    bw->bitsInBuffer = 0;
}

void bitWriterWriteBit(BitWriter* bw, int bit)
{
    bw->buffer = (bw->buffer << 1) | (bit & 1);
    bw->bitsInBuffer++;

    if (bw->bitsInBuffer == 8) {
        fwrite(&bw->buffer, 1, 1, bw->file);
        bw->buffer = 0;
        bw->bitsInBuffer = 0;
    }
}

void bitWriterWriteBits(BitWriter* bw, uint32_t bits, int numBits)
{
    for (int i = numBits - 1; i >= 0; i--) {
        int bit = (bits >> i) & 1;
        bitWriterWriteBit(bw, bit);
    }
}

void bitWriterFlush(BitWriter* bw)
{
    if (bw->bitsInBuffer > 0) {
        bw->buffer <<= (8 - bw->bitsInBuffer);
        fwrite(&bw->buffer, 1, 1, bw->file);
        bw->buffer = 0;
        bw->bitsInBuffer = 0;
    }
}

void bitReaderInit(BitReader* br, FILE* file)
{
    br->file = file;
    br->buffer = 0;
    br->bitsInBuffer = 0;
}

int bitReaderReadBit(BitReader* br)
{
    if (br->bitsInBuffer == 0) {
        int bytesRead = fread(&br->buffer, 1, 1, br->file);
        if (bytesRead == 0) {
            return -1;
        }
        br->bitsInBuffer = 8;
    }
    br->bitsInBuffer--;
    int bit = (br->buffer >> br->bitsInBuffer) & 1;
    return bit;
}

uint32_t bitReaderReadBits(BitReader* br, int numBits)
{
    uint32_t result = 0;
    for (int i = numBits - 1; i >= 0; i--) {
        int bit = bitReaderReadBit(br);
        if (bit == -1) {
            return result;
        }
        result |= (bit << i);
    }
    return result;
}

int bitReaderEof(BitReader* br)
{
    if (br->bitsInBuffer > 0) {
        return 0;
    }
    int c = fgetc(br->file);
    if (c != EOF) {
        ungetc(c, br->file);
        return 0;
    }
    return 1;
}