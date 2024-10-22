// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// sruct to save info about number
typedef struct {
    int value;
    int start; // Начальный индекс в строке
    int length; // Длина числа в символах
} Number;

void back_max(char *str) {
    Number numbers[1000]; // mac_len = 1000
    int count = 0;
    int i = 0;
    int len = strlen(str);
    
    while (i < len) {
        // Пропускаем запятые
        if (str[i] == ',') {
            i++;
            continue;
        }
        
        // Start Number
        int start = i;
        int sign = 1;
        if (str[i] == '-') {
            sign = -1;
            i++;
        }
        
        // read digits
        int num = 0;
        while (i < len && str[i] >= '0' && str[i] <= '9') {
            num = num * 10 + (str[i] - '0');
            i++;
        }
        num *= sign;
        int end = i; // End Number
        
        // Save Info about NUMBER
        numbers[count].value = num;
        numbers[count].start = start;
        numbers[count].length = end - start;
        count++;
    }
    
    if (count == 0) {return;}
    
    // Search max and last count max
    int max = INT_MIN;
    int max_idx = -1;
    for (int j = 0; j < count; j++) {
        if (numbers[j].value > max) {
            max = numbers[j].value;
            max_idx = j;
        } else if (numbers[j].value == max) {
            max_idx = j; 
        }
    }
    
    // Max number in end str or no number in str
    if (max_idx == -1 || max_idx == count - 1) {return;}
    
    // Make help string
    char temp[10000] = {0};
    int pos = 0;
    
    for (int j = 0; j < count; j++) {
        if (j == max_idx) {
            continue;
        }
        int num_length = numbers[j].length;
        memcpy(temp + pos, str + numbers[j].start, num_length);
        pos += num_length;
        if (j != count -1) {
            temp[pos] = ',';
            pos++;
        }
    }
    
    // add , 
    if (pos > 0 && temp[pos -1] != ',') {
        temp[pos] = ',';
        pos++;
    }
    
    // Add max at end str
    memcpy(temp + pos, str + numbers[max_idx].start, numbers[max_idx].length);
    pos += numbers[max_idx].length;
    temp[pos] = '\0';
    
    // copy
    strcpy(str, temp);
}

int main(void){
    char str2[100] = "5,-14,5,11,2,3";
    printf("Исходная строка: %s\n", str2);
    back_max(str2);
    printf("Преобразованная строка: %s\n", str2);
    
    return 0;
}
