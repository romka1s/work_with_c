#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE 50

typedef struct {
    char surname[MAX_SIZE];
    char name[MAX_SIZE];
    char patronymic[MAX_SIZE];
    time_t birthdate;
} Person;

int is_valid_date(int day, int month, int year) {
    struct tm tm = {0};
    tm.tm_mday = day;
    tm.tm_mon = month - 1;
    tm.tm_year = year - 1900; // Years > 1900

    if (mktime(&tm) == -1) {
        return 0;
    }

    if (tm.tm_mday != day || tm.tm_mon != month - 1 || tm.tm_year != year - 1900) {
        return 0;
    }

    return 1;
}

int main() {
    char filename[256] = "out.bin";
    FILE *fp;

    // printf("Enter the name of the binary file to write to: ");
    // scanf("%255s", filename);

    fp = fopen(filename, "wb");
    if (fp == NULL) {
        perror("Error opening file");
        exit(1);
    }

    printf("Enter data (Surname Name FatherName DD MM YYYY), one per line.\n");
    printf("Press Ctrl+D or Cntrl+Z to finish.\n");

    while (1) {
        Person person;
        int day, month, year;
        int n;

        printf("Input: ");
        n = scanf("%49s %49s %49s %d %d %d", person.surname, person.name, person.patronymic, &day, &month, &year);
        if (n == EOF) {
            break;
        }
        if (n != 6) {
            fprintf(stderr, "Invalid input format.\n");
            // Clear input buffer
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        if (!is_valid_date(day, month, year)) {
            fprintf(stderr, "Invalid date.\n");
            continue;
        }

        // Convert to time_t
        struct tm tm = {0};
        tm.tm_mday = day;
        tm.tm_mon = month - 1;
        tm.tm_year = year - 1900;

        person.birthdate = mktime(&tm);

        // Write the structure to binary file
        if (fwrite(&person, sizeof(Person), 1, fp) != 1) {
            perror("Error writing to file");
            fclose(fp);
            exit(EXIT_FAILURE);
        }
    }

    fclose(fp);
    return 0;
}
