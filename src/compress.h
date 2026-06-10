#pragma once

#include <stdint.h>
#include <stdio.h>

// Сжимает файл input в output по алгоритму Хаффмана
// Возвращает 0 при успехе, -1 при ошибке
int compressFile(const char* inputPath, const char* outputPath);