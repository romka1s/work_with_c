#include <stdio.h>

int main(void)
{
    unsigned char a, b;
    if (scanf("%c %c", &a, &b) < 2) {perror("Input ERR"); return 1;}
    unsigned char c = a & b;
    if (c == b) {printf("yes");} else {printf("no");}
    return 0;
}