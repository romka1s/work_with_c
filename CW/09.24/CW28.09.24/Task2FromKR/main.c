#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

void back_max(char *str){
    int i = 0, sign = 1, num = 0, max = INT_MIN, last_ind = 0, len_dig = 0, len_max = 0;
    char *a;
    char *maxs;
    while (str[i] != '\0'){
        if (str[i] == '-') {sign = -1;}
        if (str[i] >= '0' && str[i] <= '9'){
            if (num == 0) {char p = str[i];}
            num = num * 10 + (str[i] - '0');
            len_dig ++;
        }
        if (str[i] == ','){
            num = num * sign;
            if (num >= max) {max = num;last_ind = i;len_max = len_dig; maxs = a;}
            printf("%d", num);
            num = 0; sign = 1; len_dig = 0;
        }
        i ++;
    }
    printf("%d\n", num);
    
    printf("%c", *maxs);
    printf("%d  %d  %d", max, last_ind, len_max);
}

int main(void){
    char *str = "5,-14,5,11,2,3";
    back_max(str);
    return 0;
}