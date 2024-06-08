#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024

int client_sockets[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *arg);
void send_message_to_all(char *message, int sender_sock);
void send_confirmation(int client_sock, char *message);

int main(int argc, char **argv) {
    int server_sock, client_sock, port, addr_len;
    struct sockaddr_in server_addr, client_addr;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    port = atoi(argv[1]);

    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(&(server_addr.sin_zero), 0, 8);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, MAX_CLIENTS) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", port);

    addr_len = sizeof(struct sockaddr_in);

    while (1) {
        if ((client_sock = accept(server_sock, (struct sockaddr *)&client_addr, (socklen_t *)&addr_len)) == -1) {
            perror("accept");
            continue;
        }

        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, (void *)(intptr_t)client_sock) != 0) {
            perror("pthread_create");
            close(client_sock);
        }
    }

    close(server_sock);
    return 0;
}

void *handle_client(void *arg) {
    int client_sock = (int)(intptr_t)arg;
    char buffer[BUFFER_SIZE];
    int nbytes;

    // Add client to the client list
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] == 0) {
            client_sockets[i] = client_sock;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    while ((nbytes = recv(client_sock, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[nbytes] = '\0';
        send_message_to_all(buffer, client_sock);
        send_confirmation(client_sock, buffer);
    }

    // Remove client from the client list
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] == client_sock) {
            client_sockets[i] = 0;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    close(client_sock);
    return NULL;
}

void send_message_to_all(char *message, int sender_sock) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] != 0 && client_sockets[i] != sender_sock) {
            if (send(client_sockets[i], message, strlen(message), 0) == -1) {
                perror("send");
            }
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void send_confirmation(int client_sock, char *message) {
    char confirmation[BUFFER_SIZE];
    snprintf(confirmation, BUFFER_SIZE, "Message received: %s", message);
    if (send(client_sock, confirmation, strlen(confirmation), 0) == -1) {
        perror("send confirmation");
    }
}
