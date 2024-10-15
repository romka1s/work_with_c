#include <stdio.h>

int main(void)
{
    unsigned char bits;
    scanf("%hhu", &bits);
    const char mask = 0b00101000;
    char bit;
    bit = bits & mask;
    if (bit == mask){
        printf("%d", bits);
    } else {printf("-1");}

    return 0;
}