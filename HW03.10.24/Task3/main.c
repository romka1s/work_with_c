// main.c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(void) {
    int N, M;
    // Read input N M
    if (scanf("%d %d", &N, &M) != 2) {fprintf(stderr, "INPUT ERROR");return 1;}

    // check n and m
    if (N < 0 || N > 1000 || M < 0 || M > 1000) {fprintf(stderr, "INPUT ERR: N and M is int from 0 to 1000\n");return 1;}

    // Give memory: uint64_t - type data : unsigned int with len 64bit
    uint64_t X[1000];
    uint64_t Y[1000];

    // Read N numbers X[i] in 8 notation
    for (int i = 0; i < N; i++) {
        unsigned long long temp;
        if (scanf("%llo", &temp) != 1) {fprintf(stderr, "Input Err X[%d]\n", i);return 1;}
        X[i] = (uint64_t)temp;
    }

    // Read M numbers Y[j] in 8 notation
    for (int j = 0; j < M; j++) {
        unsigned long long temp;
        if (scanf("%llo", &temp) != 1) {fprintf(stderr, "Input Err Y[%d]\n", j);return 1;}
        Y[j] = (uint64_t)temp;
    }

    // Read S in 8 notation
    unsigned long long temp_S;
    if (scanf("%llo", &temp_S) != 1) {fprintf(stderr, "Input Err S\n");return 1;}
    __int128 S = (__int128)temp_S; // int with len 128 bit

    int max_i = -1;
    int max_j = -1;

    // Search max lexographic pare 
    // going from the end => first pare - max lexographic pare
    for (int i = N - 1; i >= 0; i--) {
        for (int j = M - 1; j >= 0; j--) {
            __int128 sum = (__int128)X[i] + (__int128)Y[j];
            if (sum == S) {
                max_i = i;
                max_j = j;
                break;
            }
        }
        if (max_i != -1) {break;}
    }

    // Print RES
    if (max_i != -1 && max_j != -1) {printf("%d %d\n", max_i, max_j);} 
    else {printf("-1 -1\n");}

    return 0;
}
