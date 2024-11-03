#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
// int pipe(int fd[2]);

int main(int argc, char * argv[]){
    __pid_t pid;
    int arr[2], n;
    char line[12];
    if (pipe(arr) < 0){
        perror("Pipe err"); 
        exit(1);
    }

    if ((pid = fork()) < 0){
        perror("Fork err"); 
        exit(1);
    }
    else if (pid > 0){ // parrent
        close(arr[0]);
        write(arr[1], "hello world\n", 12);
    }
    else {
        close(arr[1]);
        sleep(5);
        n = read(arr[0], line, 12);
        write(STDOUT_FILENO, line, n);
    }
    return 0;
}