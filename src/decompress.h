#pragma once

// Распаковывает файл, сжатый алгоритмом Хаффмана
// Возвращает 0 при успехе, -1 при ошибке
int decompressFile(const char* inputPath, const char* outputPath);