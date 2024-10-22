#include "sort_lines.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    // Проверка количества аргументов командной строки
    if (argc != 3) {
        fprintf(stderr, "Использование: %s <входной_файл> <выходной_файл>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Открытие входного файла для чтения
    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        fprintf(stderr, "Ошибка открытия входного файла '%s'.\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Чтение строк из входного файла
    int line_count = 0;
    char **lines = read_lines(input_file, &line_count);
    fclose(input_file);

    if (line_count == 0) {
        fprintf(stderr, "Входной файл пуст.\n");
        free(lines);
        return EXIT_FAILURE;
    }

    // Сортировка строк в лексикографическом порядке
    qsort(lines, line_count, sizeof(char *), compare_strings);

    // Открытие выходного файла для записи
    FILE *output_file = fopen(argv[2], "w");
    if (output_file == NULL) {
        fprintf(stderr, "Ошибка открытия выходного файла '%s'.\n", argv[2]);
        free_lines(lines, line_count);
        return EXIT_FAILURE;
    }

    // Запись отсортированных строк в выходной файл
    for (int i = 0; i < line_count; i++) {
        fprintf(output_file, "%s\n", lines[i]);
    }

    fclose(output_file);

    // Освобождение памяти
    free_lines(lines, line_count);

    return EXIT_SUCCESS;
}
