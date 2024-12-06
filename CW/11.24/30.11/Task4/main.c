#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int check_status(int status) {
    if (WIFEXITED(status))
        if (WEXITSTATUS(status) == 0)
            return 0;

    return 1;
}
int main(int argc, char * argv[]){
    int fd[2];
    pipe(fd);

    if (fork() > 0){
        close(fd[0]);
        dup2(fd[1], 1);
        close(fd[1]);
        execlp("cat", "cat", "1.txt", NULL);
        perror("ececlp");
        exit(EXIT_FAILURE);

    } else{ 
        close(fd[1]);
        int q = open("2.txt", O_WRONLY);
        if (q == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        dup2(fd[0], 0);
        dup2(q, 1);
        close(fd[0]);
        close(q);
        execlp("grep", "grep", "dima", NULL);
        perror("ececlp");
        exit(EXIT_FAILURE);
    }

    return 0;
}