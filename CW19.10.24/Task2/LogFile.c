#include <stdio.h>
#include <time.h>
#include <unistd.h>


void print_current_time(const char* msg) {
    time_t current_time;
    struct tm *time_info;
    char time_str[100];
    time(&current_time);
    time_info = localtime(&current_time);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", time_info);
    printf("%s: %s\n", msg, time_str);
}

int main(){
    print_current_time("Program started at");

    sleep(5);

    print_current_time("Program finished at");
    return 0;
}