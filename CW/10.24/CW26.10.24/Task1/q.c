#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define MAX_IPS 1024

struct IPv4 {
    unsigned int ip;
    int mask;
};

// Функция для конвертации IP-адреса из строки в целое число
unsigned int ip_to_int(const char *ip_str) {
    struct in_addr addr;
    inet_pton(AF_INET, ip_str, &addr);
    return ntohl(addr.s_addr);
}

// Функция для проверки, вложен ли IP с маской в другой IP с маской
int is_subnet_of(struct IPv4 a, struct IPv4 b) {
    if (a.mask < b.mask) return 0; // если маска a больше, то он не может быть вложен в b
    unsigned int mask = 0xFFFFFFFF << (32 - b.mask);
    return (a.ip & mask) == (b.ip & mask);
}

// Функция для считывания IP и масок из файла
int read_ips_from_file(const char *filename, struct IPv4 *ips) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка при открытии файла");
        exit(1);
    }
    
    char line[100];
    int count = 0;
    while (fgets(line, sizeof(line), file)) {
        char ip_str[20];
        int mask;
        if (sscanf(line, "%[^/]/%d", ip_str, &mask) == 2) {
            ips[count].ip = ip_to_int(ip_str);
            ips[count].mask = mask;
            count++;
        }
    }
    
    fclose(file);
    return count;
}

// Функция для удаления вложенных IP
void remove_nested_ips(struct IPv4 *ips, int *count) {
    int n = *count;
    int result_count = 0;
    struct IPv4 result[MAX_IPS];

    for (int i = 0; i < n; i++) {
        int is_nested = 0;
        for (int j = 0; j < n; j++) {
            if (i != j && is_subnet_of(ips[i], ips[j])) {
                is_nested = 1;
                break;
            }
        }
        if (!is_nested) {
            result[result_count++] = ips[i];
        }
    }

    memcpy(ips, result, result_count * sizeof(struct IPv4));
    *count = result_count;
}

int main() {
    struct IPv4 ips[MAX_IPS];
    int count = read_ips_from_file("IPv4.txt", ips);

    remove_nested_ips(ips, &count);

    printf("Оставшиеся IP:\n");
    for (int i = 0; i < count; i++) {
        struct in_addr addr;
        addr.s_addr = htonl(ips[i].ip);
        printf("%s/%d\n", inet_ntoa(addr), ips[i].mask);
    }

    return 0;
}
