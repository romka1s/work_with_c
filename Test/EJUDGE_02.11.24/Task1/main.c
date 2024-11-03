#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b) {
    int int_a = *(const int *)a;
    int int_b = *(const int *)b;
    return int_a - int_b;
}

void print(int n[4]){
    int d[3];
    for (int i = 0; i < 3; i ++)
        d[i] = n[i+1] - n[i];
    if (d[0] == d[1] && d[1] == d[2] && d[0] >=0)
        printf("%d %d\n", n[0], d[0]);
    else 
        printf("%d -1\n", n[0]);
}
int main(){
    int n[4];

    if (scanf("%d %d %d %d", &n[0], &n[1], &n[2], &n[3]) != 4){
        perror("error input");
        exit(1);
    }
    qsort(n, 4, sizeof(int), compare);

    print(n);
    return 0;
}
