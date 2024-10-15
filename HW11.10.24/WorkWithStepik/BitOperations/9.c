#include <stdio.h>

int main() {
    unsigned char flags, mask;
    scanf("%hhu %hhu", &flags, &mask);
    printf( ((flags & mask) == mask)  ? "yes" : "no");
    return 0;
}
