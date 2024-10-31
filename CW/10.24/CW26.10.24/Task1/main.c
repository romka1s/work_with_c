#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>

struct IPv4 {
    uint32_t ip;
    uint32_t mask;
};

unsigned int IPv4ToInt(const char *line) {
    int octets[4] = {0};  

    sscanf(line, "%d.%d.%d.%d", &octets[0], &octets[1], &octets[2], &octets[3]);
    
    unsigned int ip = (octets[0] << 24) | (octets[1] << 16) | (octets[2] << 8) | octets[3];
    return ip;
}

char * IntToIPv4(int){

}

void ParseIpMask(struct IPv4 **ips, const char *line, char *ip_str, int *mask, int *count) {
    char *slash = strchr(line, '/');

    size_t ip_len = slash - line;
    strncpy(ip_str, line, ip_len);
    ip_str[ip_len] = '\0';

    *mask = atoi(slash + 1);

    (*ips)[*count].ip = IPv4ToInt(ip_str);
    (*ips)[*count].mask = *mask;
    (*count)++;
}

int ReadIPv4FromFile(const char *filename, struct IPv4 **ips) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("err open file");
        exit(1);
    }

    int capacity = 100;  // Начальная вместимость
    *ips = malloc(capacity * sizeof(struct IPv4));
    if (*ips == NULL) {
        perror("err malloc");
        exit(1);
    }

    char line[100];
    int count = 0;

    while (fgets(line, sizeof(line), file)) {
        char ip_str[20];
        int mask;
        
        if (count >= capacity) {
            capacity *= 2;
            struct IPv4 *temp = realloc(*ips, capacity * sizeof(struct IPv4));
            if (temp == NULL) {
                perror("err realloc");
                free(*ips);
                exit(1);
            }
            *ips = temp;
        }

        ParseIpMask(ips, line, ip_str, &mask, &count);
    
    }
    
    fclose(file);
    return count;
}

int IFaINb(struct IPv4 a, struct IPv4 b) {
    if (a.mask > b.mask){
        int mask = 0xFFFFFFFF << (32 - b.mask);
        if ((a.ip & mask) == (b.ip & mask)){
            return 1;
        } 
        else {
            return 0;
        }
    }
    else {
        return 0;
    }
}

struct IPv4 *DelIElem(struct IPv4 **ips, int *count, int i) {
    for (int j = i; j < *count - 1; j++) {
        (*ips)[j] = (*ips)[j + 1];
    }

    (*count)--;

    struct IPv4 *temp = realloc(*ips, (*count) * sizeof(struct IPv4));
    if (temp != NULL || *count == 0) {
        *ips = temp;
    }

    return *ips;
}

void DelInserted(struct IPv4 **ips, int *count) {
    int *red = calloc(*count, sizeof(int));
    if (red == NULL) {
        perror("Error allocating memory");
        exit(1);
    }


    for (int i = 0; i < *count; i++) {
        for (int j = 0; j < *count; j++) {
            if (i != j && IFaINb((*ips)[i], (*ips)[j])) {
                red[i] = 1;
                break;
            }
        }
    }

    int new_count = 0;
    for (int i = 0; i < *count; i++) {
        if (red[i] == 0) {
            new_count++;
        }
    }

    struct IPv4 *new_ips = malloc(new_count * sizeof(struct IPv4));
    if (new_ips == NULL) {
        perror("Error allocating memory");
        free(red);
        exit(1);
    }

    int idx = 0;
    for (int i = 0; i < *count; i++) {
        if (red[i] == 0) {
            new_ips[idx] = (*ips)[i];
            idx ++;
        }
    }

    free(*ips);
    free(red);

    *ips = new_ips;
    *count = new_count;
}

int main(int argc, char *argv[]) {
    struct IPv4 *ips = NULL;
    char *filename = "IPv4.txt";

    int len = ReadIPv4FromFile(filename, &ips);

    // for (int i = 0; i < len; i++) {
    //     printf("IP %d: %u, Mask: %d\n", i + 1, ips[i].ip, ips[i].mask);
    // }

    DelInserted(&ips, &len);

    for (int i = 0; i < len; i++) {
        printf("IP %d: %u, Mask: %d\n", i + 1, ips[i].ip, ips[i].mask);
    }

    free(ips);
    return 0;
}