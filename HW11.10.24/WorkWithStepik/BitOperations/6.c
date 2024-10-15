#include <stdio.h>

int main(void)
{
    unsigned int b;
    if (scanf("%d", &b) < 1) {perror("Input ERR"); return 1;}
    b = b >> 3;
    printf("%d", b);
    return 0;
}