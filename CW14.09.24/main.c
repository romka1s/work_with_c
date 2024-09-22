#include <stdio.h>
#include <limits.h>

unsigned char shift_cycle(unsigned char a, unsigned int shift) {
    return (a >> shift) | (a << (CHAR_BIT - shift));
}

int main() {
    int n;
    unsigned char a;
    scanf("%d\n", &n);
    scanf("%hhu", &a);
    unsigned char b = shift_cycle(a, n);
    printf("%hhu\n", b);
    return 0;
}