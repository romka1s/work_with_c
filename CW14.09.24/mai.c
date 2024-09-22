#include <stdio.h>
#include <limits.h>

int main() {
    unsigned char a;
    scanf("%hhu", &a);
    printf("%p", &a);
    unsigned char *p;
    p = &a;
    printf("%p", p);
    printf("%hhu", a);
    printf("%hhu", *p);
    return 0;
}