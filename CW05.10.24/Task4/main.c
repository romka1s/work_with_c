#include <stdio.h>
#include <limits.h>
#include <stddef.h>
#include <time.h>
#include <stdlib.h> 

#define ROWS 1024
#define COLS 2048
#define ERR_MALLOC printf("Memory error"); free(A1); free(A2); free(C); exit(1);

int main() {
    srand(time(NULL));
    struct timespec t1;
    timespec_get(&t1, TIME_UTC);

    double **A1 = malloc(ROWS * sizeof(double*));
    double **A2 = malloc(COLS * sizeof(double*));
    double **C = malloc(ROWS * sizeof(double*));
    if (A1 == NULL || A2 == NULL || C == NULL) {ERR_MALLOC}

    for (int i = 0; i < ROWS; i++) {
        A1[i] = malloc(COLS * sizeof(double));
        C[i] = malloc(ROWS * sizeof(double));
        if (A1[i] == NULL || C[i] == NULL) {ERR_MALLOC}
    }

    for (int j = 0; j < COLS; j++) {
        A2[j] = malloc(ROWS * sizeof(double));
        if (A2[j] == NULL) {ERR_MALLOC}
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            A1[i][j] = rand() % 100;
            A2[j][i] = rand() % 100;
        }
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < ROWS; j++) {
            C[i][j] = 0;
        }
    }

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < ROWS; j++) {
            for (int k = 0; k < COLS; k++) {
                C[i][j] += A1[i][k] * A2[k][j];
            }
        }
    }
    struct timespec t2;
    timespec_get(&t2, TIME_UTC);

    printf("%ld s %ld ns", t1.tv_sec, t1.tv_nsec);
    printf("%ld s %ld ns", t2.tv_sec, t2.tv_nsec);

    return 0;
}