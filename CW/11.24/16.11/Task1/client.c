#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>      
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define MAX_IP_LEN 45         
#define BUFFER_SIZE 10      

int main(int argc, char *argv[]){
    int sockfd;
    struct sockaddr_in servaddr;

    char IP[MAX_IP_LEN];
    unsigned short PORT;

    printf("Введите IP-адрес сервера: ");
    if (scanf("%44s", IP) != 1){
        perror("err input IP");
        exit(EXIT_FAILURE);
    }

    printf("Введите порт сервера: ");
    if (scanf("%hu", &PORT) != 1){
        perror("Ошибка при вводе порта");
        exit(EXIT_FAILURE);
    }

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        perror("err socket");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, IP, &servaddr.sin_addr) <= 0){
        perror("err inte_pton");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
        perror("err connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    char send_buf[BUFFER_SIZE];
    char recv_buf[BUFFER_SIZE];

    if (fgets(send_buf, sizeof(send_buf), stdin) == NULL){
        printf("err input");
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_sent = send(sockfd, send_buf, strlen(send_buf), 0);
    if (bytes_sent < 0){
        perror("err send");
        exit(EXIT_FAILURE);
    } else if (bytes_sent < strlen(send_buf)){
        
    }
    
    memset(recv_buf, 0, BUFFER_SIZE);
    ssize_t bytes_received = recv(sockfd, recv_buf, BUFFER_SIZE - 1, 0);
    if (bytes_received < 0){
        perror("err answer");
        exit(EXIT_FAILURE);
    } else if (bytes_received == 0){
        printf("err server\n");
        exit(EXIT_FAILURE);
    }

        printf("server: %s\n", recv_buf);

    close(sockfd);
    return 0;
}
