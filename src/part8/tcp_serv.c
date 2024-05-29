#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define MAX_TEXT 512
#define LOG_MSG_TYPE 1

typedef struct {
    long msg_type;
    char msg_text[MAX_TEXT];
} log_msg_t;

void log_client_connection(int msgid, struct sockaddr_in client_addr) {
    log_msg_t log_msg;
    log_msg.msg_type = LOG_MSG_TYPE;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%a %b %d %H:%M:%S %Y", t);

    snprintf(log_msg.msg_text, MAX_TEXT, "new client from (%s) - %s",
             inet_ntoa(client_addr.sin_addr), timestamp);

    msgsnd(msgid, &log_msg, sizeof(log_msg.msg_text), 0);
}

void log_to_file(const char *logfile, const char *message) {
    FILE *file = fopen(logfile, "a");
    if (file == NULL) {
        perror("fopen");
        exit(1);
    }
    fprintf(file, "%s\n", message);
    fclose(file);
}

void log_manager() {
    key_t key = ftok("logfile", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    log_msg_t log_msg;
    const char *logfile = "server.log";

    while (1) {
        msgrcv(msgid, &log_msg, sizeof(log_msg.msg_text), LOG_MSG_TYPE, 0);
        log_to_file(logfile, log_msg.msg_text);
    }

    msgctl(msgid, IPC_RMID, NULL);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    key_t key = ftok("logfile", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);

    // 소켓 생성
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 소켓 옵션 설정
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // 서버 주소 설정
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 바인드
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 리슨
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // fork()를 이용하여 로그 관리 프로세스 생성
    pid_t pid = fork();
    if (pid == 0) {
        // 자식 프로세스는 로그 관리 프로세스 실행
        log_manager();
        exit(0);
    }

    // 메인 서버 루프
    while (1) {
        // 클라이언트 연결 수락
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // 클라이언트 접속 로그 기록
        log_client_connection(msgid, address);

        // 클라이언트와의 통신 처리
        // ...

        close(new_socket);
    }

    // 메시지 큐 삭제
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}
