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

int compare_age(const void *a, const void *b) {
    const Person *p1 = (const Person*)a;
    const Person *p2 = (const Person*)b;

    // For decreasing age, compare birthdates in ascending order
    if (p1->birthdate < p2->birthdate) return -1;
    if (p1->birthdate > p2->birthdate) return 1;
    return 0;
}

int main() {
    char filename[256] = "out.bin";
    FILE *fp;
    Person *persons = NULL;
    size_t count = 0;

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror("Error opening file");
        exit(1);
    }

    // Determine the number of records
    fseek(fp, 0, SEEK_END);
    long filesize = ftell(fp);
    rewind(fp);

    if (filesize % sizeof(Person) != 0) {
        perror("File size is not a multiple of record size.\n");
        fclose(fp);
        exit(1);
    }

    count = filesize / sizeof(Person);
    if (count == 0) {
        perror("File is empty.\n");
        fclose(fp);
        exit(1);
    }

    persons = malloc(count * sizeof(Person));
    if (persons == NULL) {
        perror("Memory allocation error");
        fclose(fp);
        exit(1);
    }

    if (fread(persons, sizeof(Person), count, fp) != count) {
        perror("Error reading from file");
        fclose(fp);
        free(persons);
        exit(1);
    }

    fclose(fp);

    qsort(persons, count, sizeof(Person), compare_age);

    // Output the sorted data
    printf("\nSorted data by age:\n");

    time_t current_time = time(NULL);
    for (size_t i = 0; i < count; i++) {
        Person *p = &persons[i];

        // 
        double seconds_lived = difftime(current_time, p->birthdate);
        int days_lived = (int)(seconds_lived / (60 * 60 * 24));

        // 
        struct tm *birth_tm = localtime(&p->birthdate);
        char birth_date_str[20];
        strftime(birth_date_str, sizeof(birth_date_str), "%d-%m-%Y", birth_tm);

        printf("%s %s %s, Date of Birth: %s, Days Lived: %d\n",
            p->surname, p->name, p->patronymic, birth_date_str, days_lived);
    }

    free(persons);

    return 0;
}
