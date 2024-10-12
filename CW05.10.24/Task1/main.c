#include <stdio.h>
#include <limits.h>
#include <stddef.h>
struct test{
    unsigned char a;
    int b;
    unsigned c;
    int d;
} s;

typedef struct test test_s;

struct byte_s{
    unsigned char a:1; //а занимает 1 бит младший , а b все остальные 
    unsigned char b:7; // можно написать b -> size = 2; b:8 -> size = 2;
} q;

union byte_u{
    unsigned char c;  //Можно поставить вместо char другие типы не длины 8
    struct byte_s d;
} u;


int main() {
    s.a = 0xFF;
    s.b = -1;
    s.c = 0xAA;
    s.d = 8;

    test_s r;
    test_s *p;


    u.c = 0xAF;
    printf("%d  %d\n", u.d.a, u.d.b);

    printf("%zu\n", sizeof(struct byte_s));
    p = &s;
    printf("%d\n", p->a);

    printf("%zu\n", sizeof(struct test));
    printf("%p  %p  %p  %p\n", &(s.a), &(s.b), &(s.c), &(s.d));
    printf("%zu\n", offsetof(struct test, c));

    return 0;
}