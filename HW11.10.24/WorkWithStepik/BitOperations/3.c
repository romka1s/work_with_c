#include <stdio.h>

int main(void)
{
    unsigned char bits;
    scanf("%hhu", &bits);
    const char mask = 0b00111111;
    char res;
    res = bits & mask;
    printf("%d", res);
    return 0;
}