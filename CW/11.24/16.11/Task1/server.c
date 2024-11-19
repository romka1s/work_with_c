#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>      // Используйте string.h вместо strings.h
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <signal.h>

#define PORT 40000
#define BACKLOG 12
#define BUFFER_SIZE 1024

int sock_fd;

// Обработчик сигнала для корректного завершения работы сервера
void handle_sigint(int sig) {
    printf("\nЗавершение работы сервера.\n");
    close(sock_fd);
    exit(EXIT_SUCCESS);
}

int main(int argc, char *argv[]){
    // Регистрация обработчика сигнала
    signal(SIGINT, handle_sigint);

    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("err socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sock_fd, (const struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
        perror("err bind");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(sock_fd, BACKLOG) < 0){
        perror("err listen");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

    int client_fd;
    struct sockaddr_in cli;
    socklen_t len;
    char buf[BUFFER_SIZE];

    printf("Server LISTEN %d\n", PORT);

    while (1){
        len = sizeof(cli);
        client_fd = accept(sock_fd, (struct sockaddr *)&cli, &len);
        if (client_fd < 0){
            perror("err accept");
            continue;  // Продолжить принимать другие подключения
        }

        char client_ip[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, &cli.sin_addr, client_ip, sizeof(client_ip)) == NULL){
            perror("err inet_ntop");
            close(client_fd);
            continue;
        }

        printf("Подключение от %s, порт %d\n", client_ip, ntohs(cli.sin_port));

        memset(buf, 0, BUFFER_SIZE);
        ssize_t bytes_received = recv(client_fd, buf, BUFFER_SIZE - 1, 0);
        if (bytes_received < 0){
            perror("Ошибка при получении данных");
            exit(EXIT_FAILURE);
        } else if (bytes_received == 0){
            printf("client %s off.\n", client_ip);
            exit(EXIT_FAILURE);
        }

        printf("give by client with ip %s: %s\n", client_ip, buf);

        // Отправка ответа клиенту
        const char *response = "message was gived\n";
        ssize_t bytes_sent = send(client_fd, response, strlen(response), 0);
        if (bytes_sent < 0){
            perror("err 92");
            exit(EXIT_FAILURE);
        }

        close(client_fd);
    }

    close(sock_fd);
    return 0;
}
