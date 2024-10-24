#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void change_endian(unsigned long long *num) {
    unsigned long long original = *num;
    unsigned long long reversed = 0;
    for (int i = 0; i < 8; i++) {
        reversed <<= 8;
        reversed |= (original & 0xFF);
        original >>= 8;
    }
    *num = reversed;
}

void ull_to_bits(unsigned long long num, char *bits_str) {
    int started = 0; 
    int index = 0;

    for (int i = 63; i >= 0; i--) {
        char bit = (num & (1ULL << i)) ? '1' : '0';
        if (bit == '1') {
            started = 1;
        }
        if (started) {
            bits_str[index++] = bit;
        }
    }

    if (!started) {bits_str[index++] = '0';}

    bits_str[index] = '\0';
}

int main(int argc, char **argv){
    if (argc != 3) {fprintf(stderr, "use: %s <file.bin - input> <file.txt - output>\n", argv[0]);return 1;}

    FILE *f_bin = fopen(argv[1], "rb");
    if (f_bin == NULL) {fprintf(stderr, "err open bin file: %s\n", argv[1]); return 1;}

    FILE *f_text = fopen(argv[2], "w");
    if (f_text == NULL) {fprintf(stderr, "err open txt file: %s\n", argv[2]); fclose(f_bin);return 1;}

    unsigned long long number;
    size_t items_read = fread(&number, sizeof(unsigned long long), 1, f_bin);
    if (items_read != 1) {
        if (feof(f_bin)) {fprintf(stderr, "Bin file is empty or error: %s\n", argv[1]);} 
        else {fprintf(stderr, "err read bin file: %s\n", argv[1]);}
        fclose(f_bin);
        fclose(f_text);
        return 1;
    }
    fclose(f_bin);

    // Check "ENDIAN"
    char *endian_env = getenv("ENDIAN");
    if (endian_env != NULL) {
        if (strcmp(endian_env, "BIG") == 0) {
            // if "BIG", convert from big endian to little endian 
            change_endian(&number);
        } 
        else if (strcmp(endian_env, "LITTLE") != 0) {
            fprintf(stderr, "err ENDIAN: %s\n", endian_env);
            fclose(f_text);
            return 1;
        }
        // if "LITTLE", nothing, little endian
    } else {
        // if not define, use little endian 
    }

    // bumber => string of bits
    char bits_str[65];
    ull_to_bits(number, bits_str);

    if (fprintf(f_text, "%s\n", bits_str) < 0) {fprintf(stderr, "Err write to output.txt: %s\n", argv[2]);fclose(f_text);return 1;}

    fclose(f_text);
    return 0;
}