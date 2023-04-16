#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAX_MSG_SIZE 1024

int main(int argc, char *argv[]) {
    int sock_fd;
    struct sockaddr_in server_addr;
    char msg[MAX_MSG_SIZE];

    if (argc != 3) {
        fprintf(stderr, "Su dung: %s <server_ip> <server_port>\n", argv[0]);
        exit(1);
    }

    // create socket
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    // setup server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    server_addr.sin_port = htons(atoi(argv[2]));

    // connect to server
    if (connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        exit(1);
    }

    // read input from user
    printf("Nhap MSSV: ");
    fgets(msg, MAX_MSG_SIZE, stdin);
    msg[strcspn(msg, "\n")] = '\0';
    send(sock_fd, msg, strlen(msg), 0);

    printf("Nhap ho ten: ");
    fgets(msg, MAX_MSG_SIZE, stdin);
    msg[strcspn(msg, "\n")] = '\0';
    send(sock_fd, msg, strlen(msg), 0);

    printf("Nhap ngay sinh: ");
    fgets(msg, MAX_MSG_SIZE, stdin);
    msg[strcspn(msg, "\n")] = '\0';
    send(sock_fd, msg, strlen(msg), 0);

    printf("Nhap diem trung binh: ");
    fgets(msg, MAX_MSG_SIZE, stdin);
    msg[strcspn(msg, "\n")] = '\0';
    send(sock_fd, msg, strlen(msg), 0);

    printf("Da gui thong tin sinh vien den server.\n");

    // close socket
    close(sock_fd);

    return 0;
}