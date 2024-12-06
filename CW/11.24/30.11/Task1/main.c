#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/stat.h>
#include <stdint.h>
#include <inttypes.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <math.h>
#include <sys/wait.h>
#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{
    if (argc != 6) {
        fprintf(stderr, "Usage: %s <FILE> <P> <x0> <s> <K>\n", argv[0]);
        return EXIT_FAILURE;
    }

    //ввод
    const char *output_file = argv[1];
    int P = atoi(argv[2]);
    double x0 = atof(argv[3]);
    double s = atof(argv[4]);
    int K = atoi(argv[5]); 

    //проверка
    if (P <= 0 || s <= 0 || K < 0) {
        fprintf(stderr, "wrong parameters.\n");
        return EXIT_FAILURE;
    }


    //Ввод матрицы
    int stroks = K+1;
    int cols = P;

    //Создание разделяемой памяти
    int shm_id = shmget(IPC_PRIVATE, stroks*cols*sizeof(int), IPC_CREAT | 0666);
    if (shm_id <0){
        perror("warning shm");
        return EXIT_FAILURE;
    }

    //Привязка памяти
    double (*Y)[cols] = shmat(shm_id,NULL, 0);
    if (Y ==(void *) -1){
        perror("shmat");
        return EXIT_FAILURE;
    }

    // Создание процессов
    pid_t pid;
    for (int j=0; j<P; j++){
        pid = fork();
        if (pid<0){
            perror("fork");
            return EXIT_FAILURE;
        }
        if (pid == 0){
            for (int i =0; i<=K; i++){
                Y[i][j] = sin((x0 + i*s) * (j+1) / P * M_PI);
            }
            exit(0);
        }
    }

    //Завершение процессов
    for (int j=0; j<P; j++){
        wait(NULL);
    }

    //Запись матрицы в файл
    FILE *file;
    file = fopen(output_file, "w");
    if (file == NULL){
        perror("fopen");
        return EXIT_FAILURE;
    }
    for (int i =0; i<=K; i++){
        for (int j=0; j<P; j++){
            fprintf(file, "%20.10g", Y[i][j]);
        }
        fprintf(file, "\n");
    }
    fclose(file);

    //Освобождение памяти
    if (shmdt(Y) == -1){
        perror("shmdt");
        return EXIT_FAILURE;
    }
    if (shmctl(shm_id, IPC_RMID, NULL) == -1){
        perror("shmctl");
        return EXIT_FAILURE;
    }

    return 0;
}
