#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdint.h>
#include <unistd.h>
#include <signal.h>

void sig_chld(int signo) {
    printf("caught SIGCHLD\n");
}
void sig_int(int signo) {
    printf("caught SIGINT\n");
}
int main(int argc, char *argv[]){
    // int pid_t = getpid();
    // int ppid_t = getppid();
    // printf("%d\n", pid_t);
    // printf("%d\n", ppid_t);

    int var;
    __pid_t pid;
    var = 88;

    if (signal(SIGINT, sig_int) == SIG_ERR){
        printf("SIG_ERR");
    }
    if (signal(SIGCHLD, sig_chld) == SIG_ERR){
        printf("SIGSGILD");
    }

    if ((pid = fork()) < 0){
        perror("err of fork");
        exit(1);
    } else if (pid == 0){
        int pid_t = getpid();
        int ppid_t = getppid();
        // printf("son: %d %d", pid_t, ppid_t);
    } else {
        int pid_t = getpid();
        int ppid_t = getppid();
        sleep(10);
        // printf("parent: %d %d", pid_t, ppid_t);
    }
    return 0;
}