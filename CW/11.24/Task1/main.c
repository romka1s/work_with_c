#include <sys/resource.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

void Dimalog(int priority, const char *format, ...){
    FILE * file;
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];

    file = fopen("./daemon.log", "a");

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);

    va_list va;
    va_start(va, format);
    int rv = vfprintf(file, format, va);
    va_end(va);


    fprintf(file, "\n");
    fprintf(file, "%s ", buffer);

    switch (priority){
        case LOG_WARNING: fprintf(file, "WARNING\n"); break;
        case LOG_ERR: fprintf(file, "ERROR\n"); break;
        default: fprintf(file, "Other info\n");
    }
    fclose(file);

}

int main(int argc, char *argv[]){
    int fd0, fd1, fd2;
    pid_t pid;
    struct rlimit rl;

    if ((pid = fork()) < 0)
        printf("fork didnt work");
    else if (pid != 0) /* parent */
        exit(0);
    
    setsid();
    if (getrlimit(RLIMIT_NOFILE, &rl) < 0) //кол во ИД
        printf("can’t get file limit");

    if (rl.rlim_max == RLIM_INFINITY)
        rl.rlim_max = 1024; 
        printf("\n%ld\n", rl.rlim_max);
    for (int i = 0; i < rl.rlim_max; i++)
        close(i);   

    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);
    
    Dimalog(LOG_WARNING, "%d %d", 1, 2);
    sleep(5);
    Dimalog(LOG_INFO, "%d %d %d", 1, 2, 4);
    // int q;
    // scanf("%d", &q);
    printf("\nDima lox\n");
}