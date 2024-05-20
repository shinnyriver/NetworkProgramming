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

int main() {
    int serv_fd, client_fd, file_fd;
    char client_fifo[BUFFER_SIZE];
    char buffer[BUFFER_SIZE];
    request_t request;

    mkfifo(SERV_FIFO, 0666);

    while (1) {
        serv_fd = open(SERV_FIFO, O_RDONLY);
        if (serv_fd < 0) {
            perror("open serv_fifo");
            exit(1);
        }

        read(serv_fd, &request, sizeof(request));
        close(serv_fd);

        snprintf(client_fifo, BUFFER_SIZE, "fifo.%d", request.pid);

        client_fd = open(client_fifo, O_WRONLY);
        if (client_fd < 0) {
            perror("open client_fifo");
            continue;
        }


        file_fd = open(request.filepath, O_RDONLY);
        if (file_fd < 0) {
            perror("open file");
            write(client_fd, "Error: Cannot open file\n", 24);
            close(client_fd);
            continue;
        }

        int n;
        while ((n = read(file_fd, buffer, BUFFER_SIZE)) > 0) {
            write(client_fd, buffer, n);
        }

        close(file_fd);
        close(client_fd);
    }

    unlink(SERV_FIFO);
    return 0;
}