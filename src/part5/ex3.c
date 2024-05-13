#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>

void test_socket_buffer_size(int sockfd, int optname, const char *optname_str) {
    int sizes[] = {20, 100, 200, 256, 1024, 8192, 10240, 50000, 80000, 120000, 200000, 400000, 800000};
    int actual_size;
    socklen_t optlen = sizeof(actual_size);

    printf("Testing %s...\n", optname_str);

    for (int i = 0; i < sizeof(sizes) / sizeof(sizes[0]); i++) {
        int requested_size = sizes[i];
        // Set buffer size
        if (setsockopt(sockfd, SOL_SOCKET, optname, &requested_size, sizeof(requested_size)) < 0) {
            perror("setsockopt");
            continue;
        }

        // Get actual size set
        if (getsockopt(sockfd, SOL_SOCKET, optname, &actual_size, &optlen) < 0) {
            perror("getsockopt");
        } else {
            printf("Requested: %d bytes, Actual: %d bytes\n", requested_size, actual_size);
        }
    }
}

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Cannot open socket");
        exit(EXIT_FAILURE);
    }

    // Test receive buffer sizes
    test_socket_buffer_size(sockfd, SO_RCVBUF, "SO_RCVBUF");

    // Test send buffer sizes
    test_socket_buffer_size(sockfd, SO_SNDBUF, "SO_SNDBUF");

    close(sockfd);
    return 0;
}
