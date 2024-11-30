#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <limits.h>

#define COUNT_THERAD 10

pthread_t ntid; 

pthread_barrier_t b;

struct tree {
    struct tree * left;
    struct tree * right;
    long long count;
    char * string;
};

struct resh{
    long long first;
    long long last;
    pthread_t tid;
    struct tree *t;
};

void printids(const char *s){
    pid_t pid;
    pthread_t tid;

    pid = getpid();
    tid = pthread_self();
    printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long) pid, (unsigned long) tid, (unsigned long) tid);
}

void *thr_fn(void * arg){
    printids("new thread: ");
    return ((void *)0);
}

void *thread(void * ptr){
    pid_t pid;
    pthread_t tid;

    pid = getpid();
    tid = pthread_self();

    struct resh * p = (struct resh *)ptr; 
    printf("new thread: %lu ", (unsigned long) tid);

    printf("first and last: %lld %lld\n", p -> first, p -> last);

    pthread_barrier_wait(&b);
}

int main(int argc, char * argv[]){
    if (argc != 4){
        perror("Input error");
        exit(EXIT_FAILURE);
    }
    int err;

    if (err = pthread_create(&ntid, NULL, thr_fn, NULL) != 0){
        perror("Error create thread");
        exit(EXIT_FAILURE);
    }

    printids("main thread:");
    sleep(1);

    int fd;

    if ((fd = open(argv[1], O_RDONLY)) < 0){
        perror("Error open file");
        exit(EXIT_FAILURE);
    }

    struct stat st;
    if (fstat(fd, &st) == 0) {
        printf("Size file '%s': %ld byte\n", argv[1], st.st_size);
    } else {
        perror("Error fstat");
    }
    close(fd);

    long long len = st.st_size;
    struct resh array[COUNT_THERAD];
    long long deq = len / COUNT_THERAD;

    if ((fd = open(argv[1], O_RDONLY)) < 0){
        perror("Error open file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < COUNT_THERAD; i ++){    
        char buffer;
        long long cnt = 0;

        if (i == 0){
            array[i].first = 0;
        }
        else {
            array[i].first = array[i-1].last + 1;
        }

        off_t position = array[i].first + deq;
        array[i].t = NULL;

        if (position >= len || i == COUNT_THERAD - 1){
            array[i].last = len - 1;
        } else {
            ssize_t bytesRead = pread(fd, &buffer, 1, array[i].first + deq);
            if (bytesRead <= 0) {
                array[i].last = len - 1;
            } else if (isspace(buffer)){
                array[i].last = position;
            } else {
                while (!isspace(buffer) && (position + cnt) < len){
                    cnt ++;
                    bytesRead = pread(fd, &buffer, 1, position + cnt);
                    if (bytesRead <= 0){
                        break;
                    }
                }
                array[i].last = position + cnt;
                if (array[i].last >= len){
                    array[i].last = len - 1;
                }
            } 
        }
        printf("%lld %lld\n", array[i].first, array[i].last);
    }

    pthread_barrier_init(&b, NULL, COUNT_THERAD+1);

    for (int i=0; i < COUNT_THERAD; i ++){
        int err;
        if (err = pthread_create(&array[i].tid, NULL, thread, (void *) &array[i]) != 0){
            perror("Error create thread");
            exit(EXIT_FAILURE);
        }
    }

    pthread_barrier_wait(&b);

    exit(EXIT_SUCCESS);
}