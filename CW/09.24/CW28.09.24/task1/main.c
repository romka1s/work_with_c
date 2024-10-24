#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
extern char **environ;

int main(int argc, char ** argv, char (**env)){
    // printf("%d\n", argc);
    // int i = 0;
    // while (argv[i] != NULL){
    //     printf("%s\n", argv[i]);
    //     i ++;
    // }
    // printf("%p", argv[i]);


    // char * path = getenv("PATH");
    // printf("%s", path);


    // int i =0;
    // while (environ[i] != NULL){
    //     if (strlen(environ[i]) < 32){
    //         printf("%s\n", environ[i]);
    //     }
    //     i ++ ;
    // }

    int i =0;
    while (env[i] != NULL){
        if (strlen(env[i]) < 32){
            printf("%s\n", env[i]);
        }
        i ++ ;
    }
    return 0;
}