#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


uint64_t bits_to_uint64(const char *bits, size_t length) {
    uint64_t number = 0;
    for (size_t i = 0; i < length; i++) {
        number <<= 1;
        if (bits[i] == '1') {
            number |= 1;
        }
    }
    return number;
}

void change_endian(uint64_t *num) {
    uint64_t original = *num;
    uint64_t reversed = 0;
    for (int i = 0; i < 8; i++) {
        reversed <<= 8;
        reversed |= (original & 0xFF);
        original >>= 8;
    }
    *num = reversed;
}

int main(int argc, char **argv){
    if (argc != 3) {fprintf(stderr, "USE: %s <file.txt> <file.bin>\n", argv[0]);return 1;}

    FILE *f_text = fopen(argv[1], "r");
    if (f_text == NULL) {fprintf(stderr, "Err open text file: %s\n", argv[1]);fclose(f_text);return 1;}
    FILE *f_bin = fopen(argv[2], "wb");
    if (f_bin == NULL) {fprintf(stderr, "Err open bin file: %s\n", argv[2]);fclose(f_text);return 1;}

    char bits_str[1025]; //max_len = 1024
    size_t index = 0;
    int c;
    while ((c = fgetc(f_text)) != EOF && index < 1024) {
        if (c == '0' || c == '1') {
            bits_str[index] = (char)c; 
            index ++;
        }
    }
    bits_str[index] = '\0';
    fclose(f_text);

    if (index == 0) {fprintf(stderr, "Err in text file no bits.\n");fclose(f_bin);return 1;}

    // convert bit str to number
    uint64_t number = bits_to_uint64(bits_str, index);

    // Check "ENDIAN"
    char *endian_env = getenv("ENDIAN");
    if (endian_env != NULL) {
        if (strcmp(endian_env, "BIG") == 0) {
            // if "BIG", convert in big endian
            change_endian(&number);
        } 
        else if (strcmp(endian_env, "LITTLE") != 0) {
            fprintf(stderr, "ERR ENDIAN: %s\n", endian_env);
            fclose(f_bin);
            return 1;
        }
        // if "LITTLE", use little endian
    } else {
        // if not define, use little endian
    }

    // write to bin_file
    size_t items_written = fwrite(&number, sizeof(uint64_t), 1, f_bin);
    if (items_written != 1) {fprintf(stderr, "Err write to bin file.\n");fclose(f_bin);return 1;}

    fclose(f_bin);
    return 0;
}
