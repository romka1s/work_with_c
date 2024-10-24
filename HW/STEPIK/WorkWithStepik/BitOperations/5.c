#include <stdio.h>

int main(void)
{
    unsigned char bits;
    scanf("%hhu", &bits);
    const unsigned char mask = 0b00001001;
    unsigned char r;
    unsigned char res;
    // r = bits | mask;
    res = bits ^ mask;
    printf("%d", res);
    return 0;
}