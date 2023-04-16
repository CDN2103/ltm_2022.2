#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char *hello;
    FILE *welcome_file;
    FILE *log_file;

    // Kiểm tra tham số dòng lệnh
    if (argc != 4) {
        printf("Su dung: %s <port> <welcome file> <log file>\n", argv[0]);
        return 1;
    }

    // Đọc nội dung file chào
    welcome_file = fopen(argv[2], "r");
    if (welcome_file == NULL) {
        printf("Loi mo file %s\n", argv[2]);
        return 1;
    }
    fseek(welcome_file, 0, SEEK_END);
    long welcome_size = ftell(welcome_file);
    fseek(welcome_file, 0, SEEK_SET);
    hello = malloc(welcome_size);
    fread(hello, 1, welcome_size, welcome_file);
    fclose(welcome_file);

    // Tạo socket server
    if ((server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("khong the tao socket");
        return 1;
    }

    // Thiết lập địa chỉ và cổng cho socket server
    memset(&address, '0', sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(atoi(argv[1]));

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1) {
        perror("thiet lap loi");
        return 1;
    }

    // Chờ kết nối từ client
    if (listen(server_fd, 3) == -1) {
        perror("listen");
        return 1;
    }

    // Chấp nhận kết nối từ client và ghi nội dung vào file log
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) == -1) {
            perror("accept");
            continue;
        }
        write(new_socket, hello, welcome_size);
        valread = read(new_socket, buffer, BUFFER_SIZE);
        log_file = fopen(argv[3], "a");
        fwrite(buffer, 1, valread, log_file);
        fclose(log_file);
        close(new_socket);
    }

    return 0;
}