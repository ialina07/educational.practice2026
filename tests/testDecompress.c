#include "../src/compress.h"
#include "../src/decompress.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Вспомогательная функция для создания файла
static int createTestFile(const char* path, const char* content, size_t size)
{
    FILE* f = fopen(path, "wb");
    if (!f)
        return -1;
    size_t written = fwrite(content, 1, size, f);
    fclose(f);
    return (written == size) ? 0 : -1;
}

// Вспомогательная функция для сравнения файлов
static int filesAreEqual(const char* path1, const char* path2)
{
    FILE* f1 = fopen(path1, "rb");
    FILE* f2 = fopen(path2, "rb");
    if (!f1 || !f2) {
        if (f1)
            fclose(f1);
        if (f2)
            fclose(f2);
        return 0;
    }

    int c1, c2;
    do {
        c1 = fgetc(f1);
        c2 = fgetc(f2);
        if (c1 != c2) {
            fclose(f1);
            fclose(f2);
            return 0;
        }
    } while (c1 != EOF && c2 != EOF);

    fclose(f1);
    fclose(f2);
    return (c1 == EOF && c2 == EOF);
}

int main()
{
    printf("\nTesting Decompression\n\n");

    // Тест 1: пустой файл
    printf("Test 1: Empty file\n");
    createTestFile("test_empty.txt", "", 0);
    compressFile("test_empty.txt", "test_empty.huff");
    int result = decompressFile("test_empty.huff", "test_empty_restored.txt");
    if (result != 0) {
        printf("FAIL: decompressFile returned %d\n", result);
        return 1;
    }
    if (!filesAreEqual("test_empty.txt", "test_empty_restored.txt")) {
        printf("FAIL: restored file differs from original\n");
        return 1;
    }
    printf("PASS: empty file round-trip works\n\n");

    // Тест 2: один символ
    printf("Test 2: Single character\n");
    createTestFile("test_single.txt", "A", 1);
    compressFile("test_single.txt", "test_single.huff");
    result = decompressFile("test_single.huff", "test_single_restored.txt");
    if (result != 0) {
        printf("FAIL: decompressFile returned %d\n", result);
        return 1;
    }
    if (!filesAreEqual("test_single.txt", "test_single_restored.txt")) {
        printf("FAIL: restored file differs from original\n");
        return 1;
    }
    printf("PASS: single character round-trip works\n\n");

    // Тест 3: текст "abracadabra"
    printf("Test 3: String 'abracadabra'\n");
    const char* text = "abracadabra";
    size_t textLen = strlen(text);
    createTestFile("test_abra.txt", text, textLen);
    compressFile("test_abra.txt", "test_abra.huff");
    result = decompressFile("test_abra.huff", "test_abra_restored.txt");
    if (result != 0) {
        printf("FAIL: decompressFile returned %d\n", result);
        return 1;
    }
    if (!filesAreEqual("test_abra.txt", "test_abra_restored.txt")) {
        printf("FAIL: restored file differs from original\n");
        return 1;
    }
    printf("PASS: 'abracadabra' round-trip works\n\n");

    // Тест 4: русский текст
    printf("Test 4: Russian text (UTF-8)\n");
    const char* russianText = "Привет мир! Это тестовый файл для сжатия Хаффманом.";
    size_t russianLen = strlen(russianText);
    createTestFile("test_russian.txt", russianText, russianLen);
    compressFile("test_russian.txt", "test_russian.huff");
    result = decompressFile("test_russian.huff", "test_russian_restored.txt");
    if (result != 0) {
        printf("FAIL: decompressFile returned %d\n", result);
        return 1;
    }
    if (!filesAreEqual("test_russian.txt", "test_russian_restored.txt")) {
        printf("FAIL: restored file differs from original\n");
        return 1;
    }
    printf("PASS: Russian text round-trip works\n\n");

    // Тест 5: повторяющиеся символы
    printf("Test 5: Repeated characters\n");
    char repeated[10000];
    for (int i = 0; i < 10000; i++) {
        repeated[i] = 'A';
    }
    createTestFile("test_repeated.txt", repeated, 10000);
    compressFile("test_repeated.txt", "test_repeated.huff");
    result = decompressFile("test_repeated.huff", "test_repeated_restored.txt");
    if (result != 0) {
        printf("FAIL: decompressFile returned %d\n", result);
        return 1;
    }
    if (!filesAreEqual("test_repeated.txt", "test_repeated_restored.txt")) {
        printf("FAIL: restored file differs from original\n");
        return 1;
    }
    printf("PASS: repeated characters round-trip works\n\n");

    // Тест 6: проверка обработки ошибок (неверный формат)
    printf("Test 6: Invalid file format\n");
    createTestFile("test_invalid.txt", "not a huffman file", 20);
    result = decompressFile("test_invalid.txt", "test_invalid_restored.txt");
    if (result != -1) {
        printf("FAIL: should return -1 for invalid format\n");
        return 1;
    }
    printf("PASS: error handling works\n\n");

    printf("ALL DECOMPRESSION TESTS PASSED\n");
    return 0;
}