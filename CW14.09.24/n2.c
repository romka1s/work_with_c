#include <stdio.h>
#include <string.h>

void DSL(double d, char *S) {
    unsigned char *q = (unsigned char *)&d; 
    unsigned int n = sizeof(double);       
    unsigned int k = 0;              
    unsigned char b;
    for (int i = n - 1; i >= 0; --i) {
        b = q[i];
        for (int j = 7; j >= 0; --j) {
            if ((b & (1 << j)) > 0) {S[k] = '1';} else {S[k] = '0';}
            k = k + 1;
            // S[k++] = (b & (1 << j)) ? '1' : '0';
        }
    }
}
void prints(char *S) {
    unsigned int n = 8 * sizeof(double) + 1;
    for (int i = 0; i < n; i ++){
        if ((i == 0) | (i == 11)) {printf("%c ", S[i]);} else {printf("%c", S[i]);}
    }
}
int main() {
    double d;
    scanf("%lf", &d);
    char S[8 * sizeof(double) + 1];
    DSL(d, S);
    // printf("%s\n", S);
    prints(S);
    return 0;
}