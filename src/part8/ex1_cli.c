#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define SERV_FIFO "fifo.serv"
#define BUFFER_SIZE 1024

typedef struct {
    pid_t pid;
    char filepath[BUFFER_SIZE];
} request_t;

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

    snprintf(client_fifo, BUFFER_SIZE, "fifo.%d", pid);

    mkfifo(client_fifo, 0666);

    request.pid = pid;
    strncpy(request.filepath, argv[1], BUFFER_SIZE);

    serv_fd = open(SERV_FIFO, O_WRONLY);
    if (serv_fd < 0) {
        perror("open serv_fifo");
        exit(1);
    }

    write(serv_fd, &request, sizeof(request));
    close(serv_fd);

    client_fd = open(client_fifo, O_RDONLY);
    if (client_fd < 0) {
        perror("open client_fifo");
        exit(1);
    }

    int n;
    while ((n = read(client_fd, buffer, BUFFER_SIZE)) > 0) {
        write(STDOUT_FILENO, buffer, n);
    }

    close(client_fd);

    return 0;
}
