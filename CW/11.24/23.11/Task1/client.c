#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


#define PORT 8080
#define BUFFER_SIZE 1024


int main(int argc, char * argv[]){
    int sock;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE] = {0};
    const char *message = "HELLO from client!";

    // 1 create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);

    // connect

    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0){
        perror("Connect failed");
        exit(EXIT_FAILURE);
    }

    //
    send(sock, message, strlen(message), 0);
    printf("Message sent to server\n");
    read(sock, buffer, BUFFER_SIZE);
    printf("Server: %s\n", buffer);

    close(sock);
    return 0;
}