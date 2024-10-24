#include <stdio.h>
#include <limits.h>
#include <stddef.h>

int main() {
    double d[12] = {1, [5]=2};
    double *p_d1 = &(d[0]);
    double *p_d2 = &(d[5]);
    ptrdiff_t q = p_d2 - p_d1;
    printf("%td\n", q);
    char s[9] = "ABCDEF";
    for (int i = 0; i < 9; i++){
        printf("%c\n", s[i]);
    }
    printf("%p    %p    ", &s[5], &s[9]);
    printf("%p    %p\n", &d[0], &d[5]);
    return 0;
}