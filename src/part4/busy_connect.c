#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <IP> <Port> <Number of Connections>\n", argv[0]);
        exit(1);
    }

    const char *server_ip = argv[1];
    int server_port = atoi(argv[2]);
    int num_connections = atoi(argv[3]);

    if (num_connections < 1 || num_connections > 50) {
        fprintf(stderr, "Error: Number of connections must be between 1 and 50.\n");
        exit(1);
    }

    int sockets[num_connections];
    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    for (int i = 0; i < num_connections; i++) {
        sockets[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (sockets[i] == -1) {
            perror("socket");
            continue;
        }

        if (connect(sockets[i], (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
            perror("connect");
            close(sockets[i]);
            continue;
        }

        printf("Connected: Socket #%d\n", i + 1);
    }

    printf("All requested connections are made. Press enter to exit.\n");
    getchar();

    for (int i = 0; i < num_connections; i++) {
        if (sockets[i] > 0) {
            close(sockets[i]);
        }
    }

    return 0;
}
