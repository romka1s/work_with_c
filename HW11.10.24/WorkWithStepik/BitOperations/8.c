#include <stdio.h>

int main(void)
{
    unsigned int a, b;
    if (scanf("%d %d", &b, &a) < 2) {perror("Input ERR"); return 1;}
    b = b >> 1;
    a = a >> 1;
    printf("%d", a * b);
    return 0;
}