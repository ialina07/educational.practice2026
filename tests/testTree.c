#include "../src/huffmanTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Проверка свойства префиксности кодов
static int checkPrefixProperty(uint32_t codes[256], int lengths[256])
{
    for (int i = 0; i < 256; i++) {
        if (lengths[i] == 0)
            continue;
        for (int j = 0; j < 256; j++) {
            if (i == j || lengths[j] == 0)
                continue;

            // Более короткий код не должен быть префиксом более длинного
            if (lengths[i] < lengths[j]) {
                // Сдвигаем более длинный код вправо, чтобы сравнить с более коротким
                uint32_t shifted = codes[j] >> (lengths[j] - lengths[i]);
                if (codes[i] == shifted) {
                    printf("Prefix violation: code for %d is prefix of %d\n", i, j);
                    return 0;
                }
            } else if (lengths[j] < lengths[i]) {
                uint32_t shifted = codes[i] >> (lengths[i] - lengths[j]);
                if (codes[j] == shifted) {
                    printf("Prefix violation: code for %d is prefix of %d\n", j, i);
                    return 0;
                }
            } else if (lengths[i] == lengths[j] && codes[i] == codes[j]) {
                printf("Prefix violation: same code for %d and %d\n", i, j);
                return 0;
            }
        }
    }
    return 1;
}

// Подсчёт взвешенной суммы длин кодов
static int calculateWeightedSum(int frequencies[256], int lengths[256])
{
    int sum = 0;
    for (int i = 0; i < 256; i++) {
        if (frequencies[i] > 0) {
            sum += frequencies[i] * lengths[i];
        }
    }
    return sum;
}

int main()
{
    printf("\nTesting Huffman Tree\n\n");

    // Тест 1: пустые частоты (все нули)
    printf("Test 1: All frequencies zero\n");
    int freqZero[256] = { 0 };
    Node* rootZero = buildHuffmanTree(freqZero);
    if (rootZero != NULL) {
        printf("FAIL: buildHuffmanTree should return NULL for all zeros\n");
        return 1;
    }
    printf("PASS: returns NULL correctly\n\n");

    // Тест 2: один символ
    printf("Test 2: Single character\n");
    int freqSingle[256] = { 0 };
    freqSingle['A'] = 100;
    Node* rootSingle = buildHuffmanTree(freqSingle);
    if (!rootSingle) {
        printf("FAIL: cannot build tree\n");
        return 1;
    }
    uint32_t codesSingle[256];
    int lengthsSingle[256];
    generateCodes(rootSingle, codesSingle, lengthsSingle);
    if (lengthsSingle['A'] != 0) {
        printf("FAIL: single node should have length 0, got %d\n",
            lengthsSingle['A']);
        return 1;
    }
    freeHuffmanTree(rootSingle);
    printf("PASS: single character works (code length = 0)\n\n");

    // Тест 3: два символа с разными частотами
    printf("Test 3: Two characters\n");
    int freqTwo[256] = { 0 };
    freqTwo['A'] = 10;
    freqTwo['B'] = 5;
    Node* rootTwo = buildHuffmanTree(freqTwo);
    if (!rootTwo) {
        printf("FAIL: cannot build tree\n");
        return 1;
    }
    uint32_t codesTwo[256];
    int lengthsTwo[256];
    generateCodes(rootTwo, codesTwo, lengthsTwo);
    printf("  Code A: freq=10, code=%u, len=%d\n",
        codesTwo['A'], lengthsTwo['A']);
    printf("  Code B: freq=5,  code=%u, len=%d\n",
        codesTwo['B'], lengthsTwo['B']);
    // Для двух символов коды должны быть 0 и 1 (оба длины 1)
    if (lengthsTwo['A'] != 1 || lengthsTwo['B'] != 1) {
        printf("FAIL: both codes should have length 1\n");
        return 1;
    }
    if ((codesTwo['A'] == codesTwo['B']) || (codesTwo['A'] > 1 || codesTwo['B'] > 1)) {
        printf("FAIL: codes should be 0 and 1\n");
        return 1;
    }
    freeHuffmanTree(rootTwo);
    printf("PASS: two characters work\n\n");

    // Тест 4: строка "abracadabra"
    printf("Test 4: String 'abracadabra'\n");
    int freqAbra[256] = { 0 };
    freqAbra['a'] = 5;
    freqAbra['b'] = 2;
    freqAbra['r'] = 2;
    freqAbra['c'] = 1;
    freqAbra['d'] = 1;
    Node* rootAbra = buildHuffmanTree(freqAbra);
    if (!rootAbra) {
        printf("FAIL: cannot build tree\n");
        return 1;
    }
    uint32_t codesAbra[256];
    int lengthsAbra[256];
    generateCodes(rootAbra, codesAbra, lengthsAbra);

    printf("  Results:\n");
    int weightedSum = calculateWeightedSum(freqAbra, lengthsAbra);
    for (int i = 0; i < 256; i++) {
        if (freqAbra[i] > 0) {
            printf("    '%c' (freq=%d): code=%u (0x%X), len=%d\n",
                i, freqAbra[i], codesAbra[i], codesAbra[i], lengthsAbra[i]);
        }
    }
    printf("  Weighted sum of lengths: %d bits\n", weightedSum);

    // Ожидаем, что weighted sum <= 8 * 11 = 88 (исходный размер в ASCII)
    if (weightedSum >= 88) {
        printf("FAIL: compression ineffective (%d >= 88)\n", weightedSum);
        return 1;
    }
    printf("PASS: compression works (%d < 88)\n\n", weightedSum);

    // Тест 5: проверка свойства префиксности
    printf("Test 5: Prefix property\n");
    if (!checkPrefixProperty(codesAbra, lengthsAbra)) {
        printf("FAIL: prefix property violated\n");
        return 1;
    }
    printf("PASS: all codes are prefix-free\n\n");
    freeHuffmanTree(rootAbra);

    // Тест 6: все 256 символов с разными частотами
    printf("Test 6: All 256 symbols\n");
    int freqAll[256];
    for (int i = 0; i < 256; i++) {
        freqAll[i] = i + 1;
    }
    Node* rootAll = buildHuffmanTree(freqAll);
    if (!rootAll) {
        printf("FAIL: cannot build tree for 256 symbols\n");
        return 1;
    }
    uint32_t codesAll[256];
    int lengthsAll[256];
    generateCodes(rootAll, codesAll, lengthsAll);
    if (!checkPrefixProperty(codesAll, lengthsAll)) {
        printf("FAIL: prefix property violated for 256 symbols\n");
        return 1;
    }
    int maxLen = 0;
    for (int i = 0; i < 256; i++) {
        if (lengthsAll[i] > maxLen) {
            maxLen = lengthsAll[i];
        }
    }
    printf("  Max code length: %d bits\n", maxLen);
    printf("PASS: 256 symbols work\n\n");
    freeHuffmanTree(rootAll);

    // Тест 7: освобождение дерева (valgrind проверяет утечки)
    printf("Test 7: Free tree (no memory leaks)\n");
    int freqSmall[256] = { 0 };
    freqSmall['X'] = 5;
    freqSmall['Y'] = 3;
    freqSmall['Z'] = 2;
    Node* rootSmall = buildHuffmanTree(freqSmall);
    generateCodes(rootSmall, codesAll, lengthsAll);
    freeHuffmanTree(rootSmall);
    printf("PASS: tree freed\n\n");

    printf("ALL HUFFMAN TREE TESTS PASSED\n");
    return 0;
}