#include "sort_lines.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char * fgetline(FILE *f) {
    const int SIZE = 8;
    char *a = malloc(sizeof(char) * SIZE);
    if (a == NULL) { printf("Memory error"); exit(1); }
    int i = 0;
    int k = 1;
    char c = fgetc(f);
    if (c == EOF) { free(a); return NULL; }
    while (c != EOF && c != '\n') {
        if (i >= (k * SIZE)) {
            k++;
            char *a_tmp = realloc(a, sizeof(char) * SIZE * k); 
            if (a_tmp == NULL) { 
                printf("Memory error"); 
                free(a);
                exit(1); 
            }
            a = a_tmp;
        }
        a[i] = c;
        i++;
        c = fgetc(f);
    }
    a = realloc(a, sizeof(char)*i);
    a[i] = '\0';
    return a;
}

// Чтение всех строк из файла
char **give_mas(FILE *f, int *count) {
    const int SIZE = 8;
    int i = 0;
    int k = 1;
    char **MAS = malloc(sizeof(char*) * SIZE);
    if (MAS == NULL) { printf("Memory error"); exit(1); }
    char *a = fgetline(f);
    while (a != NULL) {
        if (i >= (SIZE * k)) {
            k++;
            char **Mas_tmp = realloc(MAS, sizeof(char*) * SIZE * k);
            if (Mas_tmp == NULL) { 
                printf("Memory error"); 
                for (int j = 0; j < i; j++) {free(MAS[j]);}
                free(MAS);
                exit(1); 
            }
            MAS = Mas_tmp;
        }
        MAS[i] = a;
        i++;
        // printf("%s\n", a);
        a = fgetline(f);
    }
    char **Mas_tmp = realloc(MAS, sizeof(char*) * (i + 1));
    if (Mas_tmp == NULL) { 
        printf("Memory error at final realloc"); 
        for (int j = 0; j < i; j++) {free(MAS[j]);}
        free(MAS);
        exit(1); 
    }
    MAS = Mas_tmp;
    MAS[i] = NULL;
    *count = i;
    return MAS;
}

// Функция сравнения строк для qsort (лексикографический порядок)
int compare_strings(const void *a, const void *b) {
    const char *str1 = *(const char **)a;
    const char *str2 = *(const char **)b;
    return strcmp(str1, str2);
}

// Освобождение памяти, занятой массивом строк
void free_lines(char **lines, int count) {
    for (int i = 0; i < count; i++) {
        free(lines[i]);
    }
    free(lines);
}
