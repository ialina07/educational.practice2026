#include "../src/compress.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Вспомогательная функция для создания файла с заданным содержимым
static int createTestFile(const char* path, const char* content, size_t size)
{
    FILE* f = fopen(path, "wb");
    if (!f) {
        return -1;
    }
    size_t written = fwrite(content, 1, size, f);
    fclose(f);
    return (written == size) ? 0 : -1;
}

// Вспомогательная функция для получения размера файла
static long getFileSize(const char* path)
{
    FILE* f = fopen(path, "rb");
    if (!f) {
        return -1;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fclose(f);
    return size;
}

int main()
{
    printf("\nTesting Compression\n\n");

    // Тест 1: пустой файл
    printf("Test 1: Empty file\n");
    createTestFile("test_empty.txt", "", 0);
    int result = compressFile("test_empty.txt", "test_empty.huff");
    if (result != 0) {
        printf("FAIL: compressFile returned %d\n", result);
        return 1;
    }
    long compressedSize = getFileSize("test_empty.huff");
    if (compressedSize != 0) {
        printf("FAIL: compressed empty file should be 0 bytes, got %ld\n", compressedSize);
        return 1;
    }
    printf("PASS: empty file compression works\n\n");

    // Тест 2: один символ
    printf("Test 2: Single character\n");
    createTestFile("test_single.txt", "A", 1);
    result = compressFile("test_single.txt", "test_single.huff");
    if (result != 0) {
        printf("FAIL: compressFile returned %d\n", result);
        return 1;
    }
    compressedSize = getFileSize("test_single.huff");
    // Заголовок ~1KB, так что файл должен быть больше исходного
    if (compressedSize <= 0) {
        printf("FAIL: compressed file size is %ld\n", compressedSize);
        return 1;
    }
    printf("  Original: 1 byte, Compressed: %ld bytes (header overhead expected)\n", compressedSize);
    printf("PASS: single character compression works\n\n");

    // Тест 3: текст "abracadabra"
    printf("Test 3: String 'abracadabra'\n");
    const char* text = "abracadabra";
    size_t textLen = strlen(text);
    createTestFile("test_abra.txt", text, textLen);
    result = compressFile("test_abra.txt", "test_abra.huff");
    if (result != 0) {
        printf("FAIL: compressFile returned %d\n", result);
        return 1;
    }
    compressedSize = getFileSize("test_abra.huff");
    printf("  Original: %zu bytes, Compressed: %ld bytes\n", textLen, compressedSize);
    if (compressedSize >= (long)textLen) {
        printf("WARNING: compressed size not smaller (expected for small text)\n");
    } else {
        printf("PASS: file compressed\n");
    }
    printf("\n");

    // Тест 4: русский текст
    printf("Test 4: Russian text (UTF-8)\n");
    const char* russianText = "Привет мир! Это тестовый файл для сжатия Хаффманом.";
    size_t russianLen = strlen(russianText);
    createTestFile("test_russian.txt", russianText, russianLen);
    result = compressFile("test_russian.txt", "test_russian.huff");
    if (result != 0) {
        printf("FAIL: compressFile returned %d\n", result);
        return 1;
    }
    compressedSize = getFileSize("test_russian.huff");
    printf("  Original: %zu bytes, Compressed: %ld bytes\n", russianLen, compressedSize);
    if (compressedSize >= (long)russianLen) {
        printf("WARNING: compressed size not smaller\n");
    } else {
        printf("PASS: file compressed\n");
    }
    printf("\n");

    // Тест 5: повторяющиеся символы (хорошо сжимается)
    printf("Test 5: Repeated characters\n");
    char repeated[10000];
    for (int i = 0; i < 10000; i++) {
        repeated[i] = 'A';
    }
    createTestFile("test_repeated.txt", repeated, 10000);
    result = compressFile("test_repeated.txt", "test_repeated.huff");
    if (result != 0) {
        printf("FAIL: compressFile returned %d\n", result);
        return 1;
    }
    compressedSize = getFileSize("test_repeated.huff");
    printf("  Original: 10000 bytes, Compressed: %ld bytes\n", compressedSize);

    if (compressedSize < 10000) {
        printf("PASS: repeated characters compressed to %ld bytes (%.1f%% of original)\n",
            compressedSize, (float)compressedSize / 10000 * 100);
    } else {
        printf("FAIL: expected compression but got %ld bytes\n", compressedSize);
        return 1;
    }
    printf("\n");

    // Тест 6: случайные данные (плохо сжимается)
    printf("Test 6: Random data\n");
    unsigned char randomData[1000];
    for (int i = 0; i < 1000; i++) {
        randomData[i] = (unsigned char)(i * 12345 % 256);
    }
    createTestFile("test_random.txt", (const char*)randomData, 1000);
    result = compressFile("test_random.txt", "test_random.huff");
    if (result != 0) {
        printf("FAIL: compressFile returned %d\n", result);
        return 1;
    }
    compressedSize = getFileSize("test_random.huff");
    printf("  Original: 1000 bytes, Compressed: %ld bytes\n", compressedSize);
    printf("PASS: random data test completed\n\n");

    // Тест 7: проверка создания файла
    printf("Test 7: Output file created\n");
    FILE* check = fopen("test_abra.huff", "rb");
    if (!check) {
        printf("FAIL: output file not created\n");
        return 1;
    }
    fclose(check);
    printf("PASS: output file exists\n\n");

    // Тест 8: несуществующий входной файл
    printf("Test 8: Non-existent input file\n");
    result = compressFile("nonexistent.txt", "output.huff");
    if (result != -1) {
        printf("FAIL: should return -1 for non-existent file\n");
        return 1;
    }
    printf("PASS: error handling works\n\n");

    // Тест 9: большой файл (10KB повторяющихся символов)
    printf("Test 9: Larger file\n");
    char largeData[10240];
    for (int i = 0; i < 10240; i++) {
        largeData[i] = (i % 2 == 0) ? 'A' : 'B';
    }
    createTestFile("test_large.txt", largeData, 10240);

    // Приблизительный замер времени
    clock_t start = clock();
    result = compressFile("test_large.txt", "test_large.huff");
    clock_t end = clock();
    double timeMs = ((double)(end - start) * 1000.0) / CLOCKS_PER_SEC;

    if (result != 0) {
        printf("FAIL: compressFile returned %d\n", result);
        return 1;
    }
    compressedSize = getFileSize("test_large.huff");
    printf("  Original: 10240 bytes, Compressed: %ld bytes (%.1f%%)\n",
        compressedSize, (float)compressedSize / 10240 * 100);
    printf("  Time: %.2f ms\n", timeMs);
    printf("PASS: larger file compression works\n\n");

    printf("ALL COMPRESSION TESTS PASSED\n");

    return 0;
}