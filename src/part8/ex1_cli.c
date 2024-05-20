#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define SERV_FIFO "fifo.serv"
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <file_path>\n", argv[0]);
        exit(1);
    }

    int serv_fd, client_fd;
    char client_fifo[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    pid_t pid = getpid();
    request_t request;

    // 클라이언트 FIFO 이름 설정
    snprintf(client_fifo, BUFFER_SIZE, "fifo.%d", pid);

    // 클라이언트 FIFO 생성
    mkfifo(client_fifo, 0666);

    // 요청 설정
    request.pid = pid;
    strncpy(request.filepath, argv[1], BUFFER_SIZE);

    // 서버 FIFO 열기
    serv_fd = open(SERV_FIFO, O_WRONLY);
    if (serv_fd < 0) {
        perror("open serv_fifo");
        exit(1);
    }

    // 요청 전송
    write(serv_fd, &request, sizeof(request));
    close(serv_fd);

    // 클라이언트 FIFO 열기
    client_fd = open(client_fifo, O_RDONLY);
    if (client_fd < 0) {
        perror("open client_fifo");
        exit(1);
    }

    // 서버로부터 파일 내용 읽기
    int n;
    while ((n = read(client_fd, buffer, BUFFER_SIZE)) > 0) {
        write(STDOUT_FILENO, buffer, n);
    }

    close(client_fd);
    unlink(client_fifo); // 클라이언트 FIFO 제거

    return 0;
}
