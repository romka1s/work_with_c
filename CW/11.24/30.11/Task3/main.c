#include <unistd.h>

int main(int argc, char * argv[]){
    int fd[2];
    pipe(fd);

    if (fork() > 0){
        close(fd[0]);
        dup2(fd[1], 1);
        close(fd[1]);
        execlp("cat", "cat", "1.txt", NULL);

    } else{ 
        close(fd[1]);
        dup2(fd[0], 0);
        close(fd[0]);
        execlp("grep", "grep", "dima", NULL);
    }

    return 0;
}