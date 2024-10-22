#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int main(int argc, char *argv[]) {
    // Проверка количества аргументов командной строки
    if (argc != 3) {
        fprintf(stderr, "Использование: %s <входной_файл> <выходной_файл>\n", argv[0]); return 1;
    }

    // open input file
    FILE *f_in = fopen(argv[1], "r");
    if (f_in == NULL) {
        fprintf(stderr, "Error opening input file '%s'\n", argv[1]);
        return 1;
    }

    // do mas in input file
    int count = 0;
    char **Mas = give_mas(f_in, &count);
    fclose(f_in);

    // ERR
    if (count == 0) {fprintf(stderr, "Input file '%s' is empty.\n", argv[1]);free(Mas);return 1;}

    // Qsort
    qsort(Mas, count, sizeof(char*), compare_strings);

    // open output file
    FILE *f_out = fopen(argv[2], "w");
    if (f_out == NULL) {
        fprintf(stderr, "Error opening output file '%s'\n", argv[2]);
        //free
        for (int i = 0; i < count; i++) {
            free(Mas[i]);
        }
        free(Mas);
        return 1;
    }

    // write qsort str in output file
    for (int i = 0; i < count; i++) {
        fprintf(f_out, "%s\n", Mas[i]);
        // free
        free(Mas[i]); 
    }
    //free
    free(Mas);
    fclose(f_out);
    return 0;
}
