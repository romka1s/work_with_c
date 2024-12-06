#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h> //+SEMOPM - количество максимальных семафоров обрабатываемых одним процессом
#include <stdio.h>
#include <semaphore.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

//-1 к семафору
// struct sembuf sb;
// sb.sem_num = 0;
// sb.sem_op = -1;
// sb.sem_flg = 0;

// if (semop(semid, &sb, 1) == -1) {
//     perror("semop P");
//     exit(1);
// }
//+1 к семафору
// sb.sem_num = 0;
// sb.sem_op = 1;
// sb.sem_flg = 0;

// if (semop(semid, &sb, 1) == -1) {
//     perror("semop V");
//     exit(1);
// }
// union semun arg;
// arg.val = 0;
// if (semctl(IPC_PRIVATE, 0, SETVAL, arg) == -1){
//     perror("Err semctl"); //init semaphore with start equal arg.val
//     exit(EXIT_FAILURE);
// }
union semun{
    int  val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

int main(int argc, char *argv[]){
    int semid;
    int N = 10;
    pid_t pid;

    if ((semid = semget(IPC_PRIVATE, N, IPC_CREAT | 0666))== -1){
        perror("Err");
        exit(EXIT_FAILURE);
    }

    union semun arg;
    arg.val = 1;

    for (int i =0; i < N; i ++){
        if (i == 1) arg.val = 1;
        if (semctl(semid, i, SETVAL, arg) == -1){
            perror("Err semctl");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < N; i ++){
        pid = fork();
        if (pid == 0){
            struct sembuf sb;
            printf("Процесс %d начался ", i+1);
            sb.sem_num = i;
            sb.sem_op = -1;
            sb.sem_flg = 0;
            if (semop(semid, &sb, 1) == -1) {
                perror("Err semop P");
                exit(1);
            }
            if (i < N-1){
                sb.sem_num = i + 1;
                sb.sem_op = 1;
                sb.sem_flg = 0;
                if (semop(semid, &sb, 1) == -1){
                    perror("Ошибка semop V");
                    exit(EXIT_FAILURE);
                }
            }
            printf("Процесс %d завершился\n", i+1);
            exit(0);
        }
        else if (pid > 0){
            wait(NULL);
        }
        else{
            perror("err of fork");
            exit(EXIT_FAILURE);
        }
    }

    if (semctl(semid, 0, IPC_RMID) == -1) {
        perror("Err semctl IPC_RMID");
        exit(EXIT_FAILURE);
    }


    return 0;
}