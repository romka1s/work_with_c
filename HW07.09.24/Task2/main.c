#include <stdio.h>
#include <stdlib.h>

// sturct for save parametrs on Algoritm Evklida
typedef struct {
    int gcd;
    int x;
    int y;
} Evk;

Evk ext_gcd(int a, int b) {
    Evk res;
    if (b == 0) {
        res.gcd = a;
        res.x = 1;
        res.y = 0;
        return res;
    } else {
        Evk temp = ext_gcd(b, a % b);
        res.gcd = temp.gcd;
        res.x = temp.y;
        res.y = temp.x - (a / b) * temp.y;
        return res;
    }
}

int main() {
    signed int a, b, c;
    
    printf("Enter a, b, c for eq a*x + b*y = c:(with space)\n");
    if (scanf("%d %d %d", &a, &b, &c) != 3) {printf("Error INPUT_DATA.\n");return 1;}
    
    if (a == 0 && b == 0) {
        if (c == 0) { printf("infinitely solution.\n"); } 
        else { printf("NO solution.\n");}
        return 0;
    }
    
    if (a == 0) {
        if (c % b != 0) {printf("NO solution.\n");} 
        else {
            int y = c / b;
            printf("1 solution: x any integer, y = %d\n", y);
        }
        return 0;
    }
    
    if (b == 0) {
        if (c % a != 0) {printf("Решений нет.\n");} 
        else {
            int x = c / a;
            printf("1 solution: x = %d, y any integer\n", x);
        }
        return 0;
    }
    
    int abs_a = abs(a);
    int abs_b = abs(b);
    Evk result = ext_gcd(abs_a, abs_b);
    
    // Check if c % НОД(a, b) == 0
    if (c % result.gcd != 0) {
        printf("NO solution.\n");
        return 0;
    }
    
    int factor = c / result.gcd;
    int x0 = result.x * factor;
    int y0 = result.y * factor;
    
    if (a < 0) x0 = -x0;
    if (b < 0) y0 = -y0;
    
    printf("1 solution: x = %d, y = %d\n", x0, y0);
    
    // Common solution:
    // x = x0 + (b / gcd(a, b)) * t
    // y = y0 - (a / gcd(a, b)) * t
    printf("Common solution:\n");
    printf("x = %d + (%d / %d) * t\n", x0, b, result.gcd);
    printf("y = %d - (%d / %d) * t\n", y0, a, result.gcd);
    printf("t — any integer.\n");
    
    return 0;
}
