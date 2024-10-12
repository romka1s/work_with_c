#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <setjmp.h>
jmp_buf env;
static int s1;
void f2(){
    printf("GO depth 2\n");
    longjmp(env, 1755);
}

void f1(){
    printf("GO depth 1\n");
    f2();
}
int main(){
    static int s2;
    int a3;
    register int r4;
    volatile int v5;

    s1 = 1; s2 = 2; a3 = 3; r4 = 4; v5 = 5;
    printf("%d %d %d %d %d\n", s1, s2, a3, r4, v5);
    switch (setjmp(env)){
        case 0:
            s1 = 11; s2 = 12;
            a3 = 13; r4 = 14;
            v5 = 15;
            printf("%d %d %d %d %d\n", s1, s2, a3, r4, v5);
            f1();
            break;
        
        default:
            break;
    }
    printf("%d %d %d %d %d\n", s1, s2, a3, r4, v5);
    return 0;
}