#include <stdio.h>
#include <stdlib.h>
char * fgetline(FILE *f){
    const int SIZE = 8;
    char *a = malloc(sizeof(char)* SIZE);
    if (a == NULL){printf("Memory error"); exit(1);}
    int i = 0;
    int k = 1;
    char c = fgetc(f);
    if (c == EOF){return NULL;}
    while (c != EOF && c != '\n'){
        if (i >= (sizeof(char) * k * SIZE)){
                k ++;
                char * a_tmp = realloc(a,sizeof(char)*SIZE*k); 
                if (a_tmp == NULL){printf("Memory error"); exit(1);} 
                else {a = a_tmp;}
            }
        a[i] = c;
        i ++;
        c = fgetc(f);
    }
    a = realloc(a, sizeof(char)*i);
    a[i] = '\0';
    return a;
}
char **give_mas(FILE * f){
    const int SIZE = 8;
    int i = 0;
    int k = 1;
    char **MAS = malloc(sizeof(char*) * SIZE);
    char *a = fgetline(f);
    if (a == NULL){printf("TextFIleNULL"); exit(1);}
    while (a != NULL){
        if (i >= (sizeof(char*) * SIZE * k)){
            k ++;
            printf("%d", i);
            char **Mas_tmp = realloc(MAS,sizeof(char*)*SIZE*k);
            if (Mas_tmp == NULL){printf("Memory error"); exit(1);}
            else {MAS = Mas_tmp;}
        }
        MAS[i] = a;
        i ++;
        printf("%s\n", a);
        a = fgetline(f);
    }
    MAS[i+1] = '\0';
    if (a == NULL){printf("EOF\n");} 
}
int compare(const void *a, const void *b){
    return strcasecmp(*(const char **)a, *(const char **)b);
}
int main(void){
    FILE *f = fopen("data.txt", "r");
    char **Mas = give_mas(f);
    qsort(Mas, 10, sizeof(char*), compare);
    fclose(f);
    return 0;
}