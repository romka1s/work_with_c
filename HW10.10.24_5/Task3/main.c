#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define MAX_NAME_LENGTH 100

struct Person {
    char full_name[MAX_NAME_LENGTH]; // ФИО
    time_t dob;                      // Дата рождения
};

// Функция для парсинга даты в формате YYYY-MM-DD и преобразования в time_t
int parse_date(const char *date_str, time_t *date) {
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    
    char *result = strptime(date_str, "%Y-%m-%d", &tm);
    if (result == NULL || *result != '\0') {return -1; } //err
    
    tm.tm_isdst = -1; 
    *date = mktime(&tm);
    
    if (*date == -1) {return -1;} //err
    return 0;
}

// Функция для расчёта разницы в днях между двумя time_t
long days_between(time_t start, time_t end) {
    const long seconds_per_day = 60 * 60 * 24;
    return (end - start) / seconds_per_day;
}

// Функция для сравнения двух структур Person по возрасту (для сортировки)
int compare_persons(const void *a, const void *b) {
    struct Person *pa = (struct Person*)a;
    struct Person *pb = (struct Person*)b;
    
    if (pa->dob < pb->dob) {return -1;}
    else if (pa->dob > pb->dob) {return 1;}
    else {return 0;}
}

// Функция для сохранения данных в бинарный файл
void save_persons(const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {perror("err open bin file");exit(1);} // err
    
    printf("Введите данные (ФИО и дату рождения в формате YYYY-MM-DD).\n");
    printf("Для завершения ввода введите EOF Ctrl+D в Unix.\n");
    
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
    while (1) {
        struct Person person;
        char dob_str[11]; // Формат YYYY-MM-DD + нулевой символ
        
        printf("ФИО: ");
        // Читаем ФИО, включая пробелы до конца строки
        if (fgets(person.full_name, sizeof(person.full_name), stdin) == NULL) {break; }// EOF или ошибка
        
        // Удаляем символ новой строки, если есть
        size_t len = strlen(person.full_name);
        if (len > 0 && person.full_name[len - 1] == '\n') {person.full_name[len - 1] = '\0';}
        
        // Проверка, что ФИО не пустое
        if (strlen(person.full_name) == 0) {printf("ФИО не может быть пустым. Попробуйте снова.\n");continue;} //err
        
        printf("Дата рождения (YYYY-MM-DD): ");
        if (scanf("%10s", dob_str) != 1) {fprintf(stderr, "Ошибка ввода даты рождения.\n");break;} //err
        
        // Пропускаем оставшиеся символы до конца строки
        while ((c = getchar()) != '\n' && c != EOF);
        
        // Парсим и проверяем дату
        if (parse_date(dob_str, &person.dob) != 0) {printf("Неверный формат даты или некорректная дата. Попробуйте снова.\n");continue;} //err
        
        // Записываем структуру в файл
        if (fwrite(&person, sizeof(struct Person), 1, file) != 1) {fprintf(stderr, "Ошибка записи в файл.\n");fclose(file);exit(1);} //err
        
        printf("Данные успешно сохранены.\n");
    }
    fclose(file);
    printf("Сохранение данных завершено.\n");
}

// Функция для чтения, сортировки и отображения данных из бинарного файла
void read_and_display_persons(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {perror("err open file read");exit(1);} //err
    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    if (file_size < 0) {fprintf(stderr, "err calc length file\n");fclose(file);exit(1);} //err
    fseek(file, 0, SEEK_SET);
    
    size_t num_records = file_size / sizeof(struct Person);
    if (num_records == 0) {printf("file is empty\n");fclose(file);return;} //err
    
    // Выделяем память для всех записей
    struct Person *persons = malloc(num_records * sizeof(struct Person));
    if (persons == NULL) {fprintf(stderr, "err of malloc\n");fclose(file);exit(1);} //errr
    
    // Считываем все записи
    size_t read_count = fread(persons, sizeof(struct Person), num_records, file);
    if (read_count != num_records) {fprintf(stderr, "err read from file.\n");free(persons);fclose(file);exit(1);} //err
    fclose(file);
    
    qsort(persons, num_records, sizeof(struct Person), compare_persons);
    
    // Получаем текущую дату
    time_t now = time(NULL);
    if (now == ((time_t)-1)) {fprintf(stderr, "Ошибка получения текущего времени.\n");free(persons);exit(1);} //err
    
    // Выводим заголовок
    printf("\n%-30s %-12s %s\n", "ФИО", "Дата Рождения", "Дней прожито");
    printf("--------------------------------------------------------------\n");
    
    // Выводим отсортированные записи
    for(size_t i = 0; i < num_records; i++) {
        struct Person p = persons[i];
        struct tm *tm_info = localtime(&p.dob);
        if (tm_info == NULL) {fprintf(stderr, "Ошибка преобразования времени.\n");continue; } //err
        
        char dob_str[11]; // Формат YYYY-MM-DD
        if (strftime(dob_str, sizeof(dob_str), "%Y-%m-%d", tm_info) == 0) {
            strcpy(dob_str, "Н/Д");
        }
        
        long lived_days = days_between(p.dob, now);
        if (lived_days < 0) {
            lived_days = 0; // Если дата рождения в будущем
        }
        
        printf("%-30s %-12s %ld\n", p.full_name, dob_str, lived_days);
    }
    
    free(persons);
}