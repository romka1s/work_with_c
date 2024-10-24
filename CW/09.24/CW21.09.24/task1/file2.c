#include <stdio.h>

int a2;
static int b2;

int fun2(void){
    double f2[4];
    register double g2; // нельзя вывести адрес
    static int d2; // нельзя вывести адрес
    printf("[%p]=f[4]", &f2);
}