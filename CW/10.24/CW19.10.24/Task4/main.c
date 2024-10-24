#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
// static const char INCLUDE[] = "const void (*signal(int sign, void (*func)(int)))(int)";

enum TOKEN_E {
    TOK_OB,        // (
    TOK_CB,        // )
    TOK_PIR,       // *
    TOK_COMMA,     // ,
    TOK_OBRACKET,  // [
    TOK_CBRACKET,  // ]
    TOK_TYPE,      // void, int, char, double
    TOK_ID,        // Name, const
    TOK_NUM,       // numbers
    TOK_OTHER      // others
};

struct TOKEN_S {
    char *s;
    enum TOKEN_E t;
};

char *give_number(const char *str, int *start, int *end) {
    int len = strlen(str);
    int i = *end;

    while (i < len && isdigit(str[i])) {
        i++;
    }

    if (i > *end) {
        int num_len = i - *end;
        char *number = malloc(num_len + 1);
        if (!number) {
            perror("malloc");
            exit(1); // Error
        }
        strncpy(number, str + *end, num_len);
        number[num_len] = '\0';
        *start = i;
        *end = i;
        return number;
    }
    return NULL;
}

char *give_letter(const char *str, int *start, int *end) {
    while (isspace(str[*start])) {
        (*start)++;
    }

    int local_start = *start;

    // Итерируем, пока символ является буквой, цифрой или '_'
    while (isalnum(str[*end]) || str[*end] == '_') {
        (*end)++;
    }

    int length = *end - local_start;
    if (length == 0) {
        return NULL;
    }

    char *d = malloc((length + 1) * sizeof(char));
    if (!d) {
        perror("malloc");
        exit(1); // Error
    }

    strncpy(d, str + local_start, length);
    d[length] = '\0';

    *start = *end;
    return d;
}

// Функция для токенизации строки
struct TOKEN_S *tokenize(const char *str, int *token_count) {
    int start = 0, end = 0;
    int capacity = 10;
    *token_count = 0;

    struct TOKEN_S *tokens = malloc(capacity * sizeof(struct TOKEN_S));
    if (!tokens) {
        perror("malloc");
        exit(1); // Error
    }

    while (str[end] != '\0') {
        // Пропуск пробелов
        if (isspace(str[end])) {
            end++;
            start = end;
            continue;
        }

        char current = str[end];

        // Проверка символов
        if (current == '(' || current == ')' || current == '*' ||
            current == ',' || current == '[' || current == ']') {

            // Расширение массива при необходимости
            if (*token_count >= capacity) {
                capacity *= 2;
                tokens = realloc(tokens, capacity * sizeof(struct TOKEN_S));
                if (!tokens) {
                    perror("realloc");
                    exit(1); // Error
                }
            }

            // Добавление токена на основе текущего символа
            if (current == '(') {
                tokens[*token_count].s = strdup("(");
                tokens[*token_count].t = TOK_OB;
            } else if (current == ')') {
                tokens[*token_count].s = strdup(")");
                tokens[*token_count].t = TOK_CB;
            } else if (current == '*') {
                tokens[*token_count].s = strdup("*");
                tokens[*token_count].t = TOK_PIR;
            } else if (current == ',') {
                tokens[*token_count].s = strdup(",");
                tokens[*token_count].t = TOK_COMMA;
            } else if (current == '[') {
                tokens[*token_count].s = strdup("[");
                tokens[*token_count].t = TOK_OBRACKET;
            } else if (current == ']') {
                tokens[*token_count].s = strdup("]");
                tokens[*token_count].t = TOK_CBRACKET;
            }

            (*token_count)++;
            end++;
            start = end;
            continue;
        } else if (isalpha(current) || current == '_') {
            // Попытка извлечь слово (тип или идентификатор)
            char *word = give_letter(str, &start, &end);
            if (word) {
                const char *types[] = {"void", "int", "char", "double"};
                int num_types = sizeof(types) / sizeof(types[0]);
                int is_type = 0;
                for (int i = 0; i < num_types; i++) {
                    if (strcmp(word, types[i]) == 0) {
                        is_type = 1;
                        break;
                    }
                }

                // Расширение массива при необходимости
                if (*token_count >= capacity) {
                    capacity *= 2;
                    tokens = realloc(tokens, capacity * sizeof(struct TOKEN_S));
                    if (!tokens) {
                        perror("realloc");
                        exit(1); // Error
                    }
                }

                tokens[*token_count].s = word;
                tokens[*token_count].t = is_type ? TOK_TYPE : TOK_ID;
                (*token_count)++;
            } else {
                end++;
                start = end;
            }
        } else if (isdigit(current)) {
            // Попытка извлечь число
            char *number = give_number(str, &start, &end);
            if (number) {
                // Расширение массива при необходимости
                if (*token_count >= capacity) {
                    capacity *= 2;
                    tokens = realloc(tokens, capacity * sizeof(struct TOKEN_S));
                    if (!tokens) {
                        perror("realloc");
                        exit(1); // Error
                    }
                }

                tokens[*token_count].s = number;
                tokens[*token_count].t = TOK_NUM;
                (*token_count)++;
            } else {
                end++;
                start = end;
            }
        } else {
            // Неизвестный символ, пропускаем
            end++;
            start = end;
        }
    }

    return tokens;
}

// Функция для вывода токенов
void print_tokens(struct TOKEN_S *tokens, int token_count) {
    printf("\n");
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: %-10s (", i, tokens[i].s);
        switch (tokens[i].t) {
            case TOK_TYPE:
                printf("TYPE");
                break;
            case TOK_ID:
                printf("IDENTIFIER");
                break;
            case TOK_OB:
                printf("OPEN_BRACKET '('");
                break;
            case TOK_CB:
                printf("CLOSE_BRACKET ')'");
                break;
            case TOK_PIR:
                printf("POINTER '*'");
                break;
            case TOK_COMMA:
                printf("COMMA ','");
                break;
            case TOK_OTHER:
                printf("OTHER");
                break;
            case TOK_OBRACKET:
                printf("OPEN_BRACKET '['");
                break;
            case TOK_CBRACKET:
                printf("CLOSE_BRACKET ']'");
                break;
            case TOK_NUM:
                printf("NUM");
                break;
            default:
                printf("UNKNOWN");
        }
        printf(")\n");
    }
}

void delElemFromMas(struct TOKEN_S **tokens, int start, int end, int *token_count) {
    int num_deleted = end - start + 1;
    int new_len = *token_count - num_deleted;

    struct TOKEN_S *tmp = malloc(new_len * sizeof(struct TOKEN_S));

    for (int i = 0, j = 0; i < *token_count; i++) {
        if (i < start || i > end) { // -1 для того чтобы убрать само имя
            tmp[j].s = strdup((*tokens)[i].s); // Копируем строку
            tmp[j].t = (*tokens)[i].t;         // Копируем тип токена
            j++;
        }
    }
    for (int i = 0; i < *token_count; i++) {
        free((*tokens)[i].s);
    }
    free(*tokens);
    *tokens = tmp;
    *token_count = new_len;
}

void findName(struct TOKEN_S **tokens, int *token_count, int *name_ind) {
    char *name;
    for (int i = 0; i < *token_count; i++) {
        if (((*tokens)[i].t == TOK_ID) && (strcmp((*tokens)[i].s, "const") != 0) && (strcmp((*tokens)[i].s, "volatile") != 0)) {
            name = (*tokens)[i].s;
            *name_ind = i;
            break;
        }
    }
    printf("%s is ", name);

    //check for const and volatile
    int c_v = 0;
    if (strcmp((*tokens)[*name_ind-1].s, "const") == 0 || strcmp((*tokens)[*name_ind-1].s, "volatile") == 0){
        c_v ++;
        printf("%s ", (*tokens)[*name_ind-1].s);
    }
    delElemFromMas(tokens, *name_ind-c_v, *name_ind, token_count);
    *name_ind -= c_v; //потому что мы удалил само имя
    // print_tokens(*tokens, *token_count);
    // printf("\n%d\n", *name_ind);
}

int findCB(struct TOKEN_S *tokens, int ind, int token_count, enum TOKEN_E t){
    int depth = 1;
    // int start = ind;
    if (t == TOK_OB){
        ind++; 
        while (ind < token_count) {
            if (tokens[ind].t == TOK_OB) {
                depth++;
            } else if (tokens[ind].t == TOK_CB) {
                depth--;
                if (depth == 0) break;
            }
            ind++;
        }
    }
    else if (t == TOK_OBRACKET){
        ind++; 
        while (ind < token_count) {
            if (tokens[ind].t == TOK_OBRACKET) {
                depth++;
            } else if (tokens[ind].t == TOK_CBRACKET) {
                depth--;
                if (depth == 0) break;
            }
            ind++;
        }
    }
    else if (t == TOK_CBRACKET){
        ind--; 
        while (ind > 0) {
            if (tokens[ind].t == TOK_CBRACKET) {
                depth++;
            } else if (tokens[ind].t == TOK_OBRACKET) {
                depth--;
                if (depth == 0) break;
            }
            ind--;
        }
    }
    else if (t == TOK_CB){
        ind--; 
        while (ind > 0) {
            if (tokens[ind].t == TOK_CB) {
                depth++;
            } else if (tokens[ind].t == TOK_OB) {
                depth--;
                if (depth == 0) break;
            }
            ind--;
        }
    }
    int inner_end = ind;
    return inner_end;
}
void def1(struct TOKEN_S **tokens, int *ind, int *token_count, int *lr) {
    if (*lr == 0){
        if ((*tokens)[*ind].t == TOK_OB){
            printf("function which return ");
            // print_tokens(*tokens, *token_count);
            int end = findCB(*tokens, *ind, *token_count, TOK_OB);
            delElemFromMas(tokens, *ind, end, token_count);
            // print_tokens(*tokens, *token_count);
            *lr = 1;
            def1(tokens, ind, token_count, lr);
        }
        if ((*tokens)[*ind].t == TOK_OBRACKET){
            printf("array with len ");
            // print_tokens(*tokens, *token_count);
            int end = findCB(*tokens, *ind, *token_count, TOK_OBRACKET);
            printf("%s of ", (*tokens)[*ind + 1].s);
            delElemFromMas(tokens, *ind, end, token_count);
            // print_tokens(*tokens, *token_count);
            *lr = 1;
            // printf("%d", *ind);
            def1(tokens, ind, token_count, lr);
        }
        if ((*tokens)[*ind].t == TOK_CB || (*tokens)[*ind].t == TOK_CBRACKET){
            *lr = 1;
            if (*ind == *token_count - 1){printf("SDF");}
            def1(tokens, ind, token_count, lr);
        }
    }
    else if (*lr == 1){ 
        
        if ((*tokens)[*ind].t == TOK_CB){
            // print_tokens(*tokens, *token_count);
            int end = findCB(*tokens, *ind, *token_count, TOK_CB);
            // printf("\n%d  %d\n", end, *ind);
            for (int i = end; i < *ind; i ++){
                if ((*tokens)[i].t == TOK_PIR) {printf("pointer to ");}
                if ((*tokens)[i].t == TOK_ID) {
                    if (strcmp((*tokens)[i].s, "const") == 0){
                        printf("const ");
                    }
                    if (strcmp((*tokens)[i].s, "volatile") == 0){
                        printf("volatile ");
                    }
                }
            }
            delElemFromMas(tokens, end, *ind, token_count);
            *lr = 0;
            *ind = end;
            // printf("\n%d  %d\n", end, *ind);
            // print_tokens(*tokens, *token_count);
            if (*ind == *token_count - 1){printf("SDF");}
            def1(tokens, ind, token_count, lr);
        }
        if ((*tokens)[*ind].t == TOK_OBRACKET){
            // print_tokens(*tokens, *token_count);
            int end = findCB(*tokens, *ind, *token_count, TOK_CBRACKET);
            // printf("\n%d  %d\n", end, *ind);
            for (int i = end; i < *ind; i ++){
                if ((*tokens)[i].t == TOK_PIR) {printf("pointer to ");}
                if ((*tokens)[i].t == TOK_ID) {
                    if (strcmp((*tokens)[i].s, "const") == 0){
                        printf("const ");
                    }
                    if (strcmp((*tokens)[i].s, "volatile") == 0){
                        printf("volatile ");
                    }
                }
            }
            delElemFromMas(tokens, end, *ind, token_count);
            *lr = 0;
            *ind = end;
            // printf("\n%d  %d\n", end, *ind);
            // print_tokens(*tokens, *token_count);
            if (*ind == *token_count - 1){printf("SDF");}
            def1(tokens, ind, token_count, lr);
        }
    }
}


void def2(struct TOKEN_S ** tokens, int * token_count){
    int cnt = 0;
    for (int i = 0; i < *token_count; i ++){
        if (strcmp((*tokens)[i].s, "*") != 0){
            cnt += 1;
        }
    }
    if (cnt != *token_count){
        for (int j = 0; j < *token_count - cnt; j++){
            printf("pointer to ");
        }
    }
    for (int i = 0; i < *token_count; i ++){
        if (strcmp((*tokens)[i].s, "*") != 0){
            printf("%s ", (*tokens)[i].s );
        }
    }
}

// void free_tokens(struct TOKEN_S *tokens, int token_count) {
//     // for (int i = 0; i < token_count; i++) {
//     //     free(tokens[i].s);
//     // }
//     // free(tokens);
// }

int main(void) {
    FILE *file = fopen("test.txt", "r");
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        return EXIT_FAILURE;
    }
    int cnt = 0;
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        // Удаляем символ новой строки, если он есть
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }
        cnt ++;
        printf("%d    Input string: %s\n", cnt, line);

        int token_count = 0;
        int left_right = 0;
        int start_ind;
        struct TOKEN_S *tokens = tokenize(line, &token_count);

        // print_tokens(tokens, token_count);
        findName(&tokens, &token_count, &start_ind);

        // print_tokens(tokens, token_count);
        // printf("%d", start_ind);
        def1(&tokens, &start_ind, &token_count, &left_right);
        def2(&tokens, &token_count);
        // print_tokens(tokens, token_count);
        // Освобождение памяти
        // free_tokens(tokens, token_count);
        printf("\n\n");
        // Обрабатываем прочитанную строку
        // printf("Прочитано: %s\n", line);
    }

    if (ferror(file)) {
        perror("Ошибка при чтении файла");
        fclose(file);
        return 1;
    }
    fclose(file);
    return 0;
}
