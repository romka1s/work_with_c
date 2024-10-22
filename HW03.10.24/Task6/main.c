#include <stdio.h>
#include <stdlib.h>
#include <string.h>


char* bytes_to_bits(unsigned char *bytes, size_t byte_count, const char *order) {
    size_t bit_count = byte_count * 8;
    char *bits_str = malloc(bit_count + 1); // +1 для нулевого символа
    if (!bits_str) {fprintf(stderr, "err of malloc\n");return NULL;} //err

    size_t index = 0;

    if (strcmp(order, "BIG") == 0) {
        for (size_t i = 0; i < byte_count; i++) {
            for (int b = 7; b >= 0; b--) {
                bits_str[index++] = (bytes[i] & (1 << b)) ? '1' : '0';
            }
        }
    } 
    else { //little
        for (size_t i = byte_count; i > 0; i--) {
            for (int b = 7; b >= 0; b--) {
                bits_str[index++] = (bytes[i-1] & (1 << b)) ? '1' : '0';
            }
        }
    }

    bits_str[index] = '\0';
    return bits_str;
}

unsigned char* bits_to_bytes(const char *bits_str, size_t bit_count, const char *order, size_t *byte_count_out) {
    size_t byte_count = (bit_count + 7) / 8;
    unsigned char *bytes = calloc(byte_count, 1);
    if (!bytes) {fprintf(stderr, "err of calloc.\n");exit(1);} //err

    size_t index = 0;
    // Big Endian: first byte - older
    if (strcmp(order, "BIG") == 0) {
        for (size_t i = 0; i < byte_count; i++) {
            for (int b = 7; b >= 0; b--) {
                if (index < bit_count) {
                    bytes[i] <<= 1;
                    if (bits_str[index++] == '1') {bytes[i] |= 1;}
                } else {
                    bytes[i] <<= 1; // Добавление нулей, если битов меньше
                }
            }
        }
    } 
    else {
        for (size_t i = byte_count; i > 0; i--) {
            for (int b = 7; b >= 0; b--) {
                if (index < bit_count) {
                    bytes[i-1] <<= 1;
                    if (bits_str[index++] == '1') {
                        bytes[i-1] |= 1;
                    }
                } else {
                    bytes[i-1] <<= 1;
                }
            }
        }
    }

    *byte_count_out = byte_count;
    return bytes;
}

int find_max_sequence_length(const char *bits_str) {
    int max_len = 0;
    int current_len = 0;

    for (size_t i = 0; bits_str[i] != '\0'; i++) {
        if (bits_str[i] == '1') {
            current_len++;
            if (current_len > max_len) {
                max_len = current_len;
            }
        } else {
            current_len = 0;
        }
    }

    return max_len;
}

void replace_max_sequences(char *bits_str, int max_len) {
    int current_len = 0;
    size_t start = 0;

    for (size_t i = 0; bits_str[i] != '\0'; i++) {
        if (bits_str[i] == '1') {
            if (current_len == 0) {
                start = i;
            }
            current_len++;
            if (current_len == max_len) {
                // Замена битов на '0'
                for (size_t j = start; j < start + max_len; j++) {
                    bits_str[j] = '0';
                }
                current_len = 0; // Продолжаем поиск
            }
        } else {
            current_len = 0;
        }
    }
}

int main(int argc, char **argv){
    if (argc != 2) {fprintf(stderr, "Use: %s <file.bin - input>\n", argv[0]);return 1;} //err

    FILE *f_bin = fopen(argv[1], "rb");
    if (f_bin == NULL) {fprintf(stderr, "err open bin file: %s\n", argv[1]);return 1;} //err

    fseek(f_bin, 0, SEEK_END);
    long file_size = ftell(f_bin);
    if (file_size < 0) {fprintf(stderr, "err take len file: %s\n", argv[1]);fclose(f_bin);return 1;} //err
    fseek(f_bin, 0, SEEK_SET);

    unsigned char *buffer = malloc(file_size);
    if (!buffer) {fprintf(stderr, "err of malloc.\n");fclose(f_bin);return 1;}

    size_t bytes_read = fread(buffer, 1, file_size, f_bin);
    if (bytes_read != (size_t)file_size) {fprintf(stderr, "err read file: %s\n", argv[1]);free(buffer);fclose(f_bin);return 1;}
    fclose(f_bin);

    char *endian_env = getenv("ENDIAN");
    const char *order = "LITTLE";

    if (endian_env != NULL) {
        if (strcmp(endian_env, "BIG") == 0) {order = "BIG";} 
        else if (strcmp(endian_env, "LITTLE") != 0) {fprintf(stderr, "err ENDIAN: %s\n", endian_env);return 1;}
        else {order = "LITTLE";}
    } else {order = "LITTLE";}

    char *bits_str = bytes_to_bits(buffer, bytes_read, order);
    if (!bits_str) {free(buffer);return 1;}

    // find max sequence '1'
    int max_len = find_max_sequence_length(bits_str);
    if (max_len == 0) {printf("NO '1' bit for change.\n");free(bits_str);free(buffer);return 0;}

    // replace max sequences '1' на '0'
    replace_max_sequences(bits_str, max_len);

    // conver bits string to bytes
    size_t new_byte_count;
    unsigned char *new_buffer = bits_to_bytes(bits_str, strlen(bits_str), order, &new_byte_count);
    if (!new_buffer) {free(bits_str);free(buffer);return 1;} //err

    free(bits_str);free(buffer);

    FILE *f_out = fopen(argv[1], "wb");
    if (f_out == NULL) {fprintf(stderr, "err open bin file: %s\n", argv[1]);free(new_buffer);return 1;} //err

    size_t bytes_written = fwrite(new_buffer, 1, new_byte_count, f_out);
    if (bytes_written != new_byte_count) {fprintf(stderr, "err write to file: %s\n", argv[1]);fclose(f_out);free(new_buffer);return 1;} //err

    fclose(f_out);free(new_buffer);

    printf("replce was finished '1': %d\n", max_len);
    return 0;
}
