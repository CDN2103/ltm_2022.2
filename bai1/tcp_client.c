#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Su dung: %s <ip address> <port>\n", argv[0]);
        exit(1);
    }
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd < 0) {
        perror("Khong the tao socket");
        exit(1);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0) {
        perror("Khong the chuyen doi dia chi IP");
        exit(1);
    }

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Khong the ket noi den server");
        exit(1);
    }

    char buffer[MAX_BUFFER_SIZE];
    int n;

    while (1) {
        printf("Nhap du lieu: ");
        fgets(buffer, MAX_BUFFER_SIZE, stdin);

        if (send(sockfd, buffer, strlen(buffer), 0) < 0) {
            perror("Khong the gui du lieu den server");
            exit(1);
        }

    }
    close(sockfd);
    return 0;
}