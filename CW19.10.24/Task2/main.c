#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
static FILE LogFile;

void print_time(const char* msg) {
    FILE *LogFile = fopen("log.txt", "a");

    time_t current_time;
    struct tm *time_info;
    char time_str[100];
    time(&current_time);
    time_info = localtime(&current_time);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", time_info);

    fprintf(LogFile, "%s: %s\n", msg, time_str);
    fclose(LogFile);
}

void on_exit_1(void){
    printf("Exit handler 1 called\n");
    print_time("Programm Exit1");
}
void on_exit_2(void){
    printf("Exit handler 2 called\n");
    print_time("Programm Exit2");
    sleep(1);
}

void prepare_file(void){
    FILE *LogFile = fopen("log.txt", "w");
    fprintf(LogFile, "START\n");
    fclose(LogFile);
}
int main(void){
    prepare_file();

    print_time("Programm Start");

    atexit(on_exit_1);
    atexit(on_exit_2);
    sleep(3);
    print_time("Programm Finish");

    printf("About to exit ...\n");
    return 0;
}