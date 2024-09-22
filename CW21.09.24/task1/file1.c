#include <stdio.h>
#include "file2.h"
#include "file1.h"
extern int a2;
extern  int d2;
int a1;
static int b1;
int main(void){
    int c1;
    static int d1;
    printf("[%p]=c1\n, [%p]=d1\n", &c1, &d1);
    printf("[%p]=a1\n, [%p]=b1\n", &a1, &b1);
    fun1();
    fun2();
    printf("[%p]=a2\n", &a2);
    // printf("[%p]=d2\n", &d2);
    return 0;
}
int fun1(void){
    double f1[3];
    register double g1; // нельзя вывести адрес
    printf("[%p]=f[3]\n", &f1);
}