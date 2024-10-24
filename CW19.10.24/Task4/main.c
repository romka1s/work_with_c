#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
static const char INCLUDE[] = "const void (*signal(int sign, void (*func)(int)))(int)";

enum TOKEN_E {
    TOK_TYPE,    // Тип данных (void, int, char, double)
    TOK_ID,      // Идентификатор (имя функции, const, volatile)
    TOK_OB,      // Открывающая скобка '('
    TOK_CB,      // Закрывающая скобка ')'
    TOK_PIR,     // Указатель '*'
    TOK_COMMA,   // Запятая ','
    TOK_OTHER    // Другие символы
};

struct TOKEN_S {
    char *s;          
    enum TOKEN_E t;    
};

char *give_letter(const char *str, int *start, int *end) {
    while (isspace(str[*start])) {
        (*start)++;
    }

    int local_start = *start;

    // Итерируем, пока символ является буквой или цифрой или '_'
    while (isalnum(str[*end]) || str[*end] == '_') {
        (*end)++;
    }

    int length = *end - local_start;
    if (length == 0) {
        return NULL;
    }

    char *d = malloc((length + 1) * sizeof(char));
    if (!d) {perror("malloc"); exit(1);} //err

    strncpy(d, str + local_start, length);
    d[length] = '\0';

    *start = *end; 
    return d;
}

// Функция для токенизации строки
struct TOKEN_S* tokenize(const char *str, int *token_count) {
    int start = 0, end = 0;
    int capacity = 10;
    *token_count = 0;

    struct TOKEN_S *tokens = malloc(capacity * sizeof(struct TOKEN_S));
    if (!tokens) {perror("malloc");exit(1);} //err

    while (str[end] != '\0') {
        // Пропускаем пробелы
        if (isspace(str[end])) {
            end++;
            start = end;
            continue;
        }

        char current = str[end];

        // Проверка на символы
        if ((current == '(')  || (current == ')') || (current == '*')){
            // Добавляем токен
            if (*token_count >= capacity) {
                capacity *= 2;
                tokens = realloc(tokens, capacity * sizeof(struct TOKEN_S));
                if (!tokens) {perror("realloc"); exit(1);} //err
            }
            if (current ==  '(') {
                tokens[*token_count].s = strdup("("); 
                tokens[*token_count].t = TOK_OB;
            } else if (current == ')') {
                tokens[*token_count].s = strdup(")");
                tokens[*token_count].t = TOK_CB;
            } else if (current == '*') {
                tokens[*token_count].s = strdup("*");
                tokens[*token_count].t = TOK_PIR;   
            } else if (current == '*') {
                tokens[*token_count].s = strdup(",");
                tokens[*token_count].t = TOK_COMMA;
            }
            (*token_count)++;
            end++;
            start = end;
            continue;
        } 
        else {
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

                // Добавляем токен
                if (*token_count >= capacity) {
                    capacity *= 2;
                    tokens = realloc(tokens, capacity * sizeof(struct TOKEN_S));
                    if (!tokens) {
                        perror("realloc");
                        exit(EXIT_FAILURE);
                    }
                }
                tokens[*token_count].s = word;
                tokens[*token_count].t = is_type ? TOK_TYPE : TOK_ID;
                (*token_count)++;
            }
            else {
                end++;
                start = end;
            }
        }
    }

    return tokens;
}

// Функция для вывода токенов
void print_tokens(struct TOKEN_S *tokens, int token_count) {
    for (int i = 0; i < token_count; i++) {
        printf("Token %d: %-10s (", i, tokens[i].s);
        switch (tokens[i].t) {
            case TOK_TYPE: printf("TYPE"); break;
            case TOK_ID: printf("IDENTIFIER"); break;
            case TOK_OB: printf("OPEN_BRACKET '('"); break;
            case TOK_CB: printf("CLOSE_BRACKET ')'"); break;
            case TOK_PIR: printf("POINTER '*'"); break;
            case TOK_COMMA: printf("COMMA ','"); break;
            case TOK_OTHER: printf("OTHER"); break;
            default: printf("UNKNOWN");
        }
        printf(")\n");
    }
}

void free_tokens(struct TOKEN_S *tokens, int token_count) {
    for (int i = 0; i < token_count; i++) {
        free(tokens[i].s);
    }
    free(tokens);
}

int main(void){
    static const char INCLUDE[] = "const void (*signal(int sign, void (*func)(int)))(int)";
    
    printf("Input string: %s\n\n", INCLUDE);
    
    int token_count = 0;
    struct TOKEN_S *tokens = tokenize(INCLUDE, &token_count);
    
    printf("Tokens:\n");
    print_tokens(tokens, token_count);
    
    free_tokens(tokens, token_count);
    
    return 0;
}