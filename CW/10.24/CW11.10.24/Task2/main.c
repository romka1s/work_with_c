#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <setjmp.h>
jmp_buf env;
void d2(void){
    printf("GO depth 2\n");
    longjmp(env, 3490); // Bail out
    printf("LEAVE depth 2\n"); // This won't happen
}

void d1(void){
    printf("GO depth 1\n");
    d2();
    printf("LEAVE depth 1\n"); // This won't happen
}
int main(){
    switch (setjmp(env)) {
        case 0:
            printf("into death, setjmp() returned 0\n");
            d1();
            printf("Returned from functions\n"); // This won't happen
            break;
        case 3490:
            printf("Bailed back to main, setjmp() returned 3490\n");
            break;
    }
    return 0;
}