#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
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

char *give_letter(int *start, int *end) {
    while (isspace(INCLUDE[*start])) {(*start)++;} // SPACE

    int local_start = *start;
    while (isalnum(INCLUDE[*end]) || INCLUDE[*end] == '_') {(*end)++;}
    
    int length = *end - local_start;
    if (length == 0) {return NULL;} 
    
    char *d = malloc((length + 1) * sizeof(char));
    if (!d) {perror("malloc");exit(1);} //err
    
    strncpy(d, INCLUDE + local_start, length);
    d[length] = '\0';
    
    *start = *end; 
    return d;
}


struct TOKEN_S * TAKE(int *token_count){
    int start = 0, end = 0;
    int capacity = 10;
    *token_count = 0;

    struct TOKEN_S *tokens = malloc(capacity * sizeof(struct TOKEN_S));
    if (!tokens) {perror("malloc"); exit(1);} // err

    while (INCLUDE[end] != '\0') {
        if (isspace(INCLUDE[end])) {
            end++;
            start = end;
            continue;
        }
        char current = INCLUDE[end];
        if ((current == '(') || (current == ')') || (current == '*') || (current == ',')){
            if (*token_count >= capacity) { //if no memory
                capacity *= 2;
                tokens = realloc(tokens, capacity * sizeof(struct TOKEN_S));
                if (!tokens) {perror("realloc");exit(1);} // err
            }
            char *symbol_str = malloc(2 * sizeof(char));
            if (!symbol_str) {perror("malloc");exit(EXIT_FAILURE);} //err
            
            symbol_str[0] = current;
            symbol_str[1] = '\0';

            tokens -> s = symbol_str;

            enum TOKEN_E q;
            switch (current) {
                case '(':
                    q = TOK_OB;
                    break;
                case ')':
                    q = TOK_CB;
                    break;
                case '*':
                    q = TOK_PIR;
                    break;
                case ',':
                    q = TOK_COMMA;
                    break; 
                default:
                    q = TOK_OTHER;
            }
            tokens -> t = TOK_CB;
            (*token_count)++;
            end++;
            start = end;
            continue;
        }
        else {}
    }
}

// void, int, char, double
// const, volatile
int main(void){
    
    return 0;
}