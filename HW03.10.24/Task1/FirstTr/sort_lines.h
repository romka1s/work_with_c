#ifndef SORT_LINES_H
#define SORT_LINES_H

#include <stdio.h>

// Функция для чтения одной строки из файла
char *fgetline(FILE *f);

// Функция для чтения всех строк из файла и формирования массива строк
char **read_lines(FILE *f, int *count);

// Функция для сравнения двух строк (для qsort)
int compare_strings(const void *a, const void *b);

// Функция для освобождения памяти, занятой массивом строк
void free_lines(char **lines, int count);

#endif // SORT_LINES_H