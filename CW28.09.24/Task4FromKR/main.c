#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
extern char **environ;

int main(int argc, char ** argv, char (**env)){
    if (argc < 2) {printf("Error not 2 paramets give\n");return 1;}
    FILE *f1 = fopen(argv[1], "r");
    if (f1 == NULL) {printf("Error opening file%s\n", argv[1]);return 1;}
    FILE *f2 = fopen(argv[2], "wb");
    if (f2 == NULL) {printf("Error opening file%s\n", argv[2]);return 1;}


    fseek(f1, 0, SEEK_END);
    long f1_size = ftell(f1);
    fseek(f1, 0, SEEK_SET);
    long kolv = f1_size / 8;
    if ((kolv * 8) != f1_size) {kolv ++;}
    // printf("%ld", kolv);

    for (int i = 1; i <= kolv; i ++){
        if (i != kolv) {
            fseek(f1, i*(-8), SEEK_END);
            for (int j = 0; j <= 7; j ++){
                char c = fgetc(f1);
                printf("%c", c);
            }
        }
        else {
            fseek(f1, 0, SEEK_SET);
            for (int j = 0; j <= (f1_size - (kolv * 8)); j ++){
                char c = fgetc(f1);
                printf("%c", c);
            }
        }
        printf("\n");
    }

    return 0;
}