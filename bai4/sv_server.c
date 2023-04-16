#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

#define MAX_MSG_SIZE 1024


char* get_current_time() {
    time_t raw_time;
    struct tm *time_info;
    static char time_str[20];

    time(&raw_time);
    time_info = localtime(&raw_time);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", time_info);

    return time_str;
}

void write_log(const char *ip_addr, const char *mssv, const char *name, const char *dob, const char *gpa, const char *filename) {
    FILE *fp;

    // open log file for append
    fp = fopen(filename, "a");
    if (fp == NULL) {
        perror("fopen");
        exit(1);
    }

    // write log message to file
    fprintf(fp, "%s %s %s %s %s\n", ip_addr, get_current_time(), mssv, name, gpa);

    // close log file
    fclose(fp);
}
void print_student_info(const char* ip_addr, const char* timestamp, const char* mssv, const char* ho_ten, const char* ngay_sinh, float diem_tb) {
    printf("%s %s %s %s %s %.2f\n", ip_addr, timestamp, mssv, ho_ten, ngay_sinh, diem_tb);
}
int main(int argc, char *argv[]) {
    int listen_fd, conn_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char mssv[MAX_MSG_SIZE], name[MAX_MSG_SIZE], dob[MAX_MSG_SIZE], gpa[MAX_MSG_SIZE], msg[MAX_MSG_SIZE];
    char log_msg[MAX_MSG_SIZE*5];
    time_t raw_time;
    struct tm *time_info;
    char time_str[20];

    if (argc != 3) {
        fprintf(stderr, "Su dung: %s <server_port> <log_file>\n", argv[0]);
        exit(1);
    }

    // create socket
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    // setup server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    // bind socket to address
    if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    // listen for connections
    if (listen(listen_fd, 5) < 0) {
        perror("listen");
        exit(1);
    }

    // accept incoming connections
    if ((conn_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
        perror("accept");
        exit(1);
    }

    // read messages from client
    recv(conn_fd, mssv, MAX_MSG_SIZE, 0);

    recv(conn_fd, name, MAX_MSG_SIZE, 0);

    recv(conn_fd, dob, MAX_MSG_SIZE, 0);

    recv(conn_fd, gpa, MAX_MSG_SIZE, 0);


    // get client address and current time
    char *ip_addr = inet_ntoa(client_addr.sin_addr);
    time(&raw_time);
    time_info = localtime(&raw_time);
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", time_info);
    // print scr
    printf("%s %s %s %s %s %s \n", ip_addr, time_str, mssv, name,dob, gpa);

    // write log message
    snprintf(log_msg, sizeof(log_msg), "%s %s %s %s %s \n", ip_addr, time_str, mssv, name, gpa);
    write_log(ip_addr, mssv, name, dob, gpa, argv[2]);

    // send response to client
    snprintf(msg, sizeof(msg), "Received data from client %s at %s", ip_addr, time_str);
    send(conn_fd, msg, strlen(msg), 0);

    // close connection
    close(conn_fd);

    return 0;
}