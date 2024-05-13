#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

void print_socket_options(int sockfd) {
    int optval;
    socklen_t optlen = sizeof(optval);

    // SO_KEEPALIVE 옵션 출력
    if (getsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &optval, &optlen) < 0) {
        perror("Error getting SO_KEEPALIVE");
    } else {
        printf("SO_KEEPALIVE: %d\n", optval);
    }
}

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in serv_addr;

    if (argc < 3) {
        fprintf(stderr,"Usage: %s <server_ip> <port>\n", argv[0]);
        exit(0);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR opening socket");
        exit(1);
    }

    // 소켓 옵션 출력: 연결 전
    printf("Before connection:\n");
    print_socket_options(sockfd);

    // 서버 주소 설정
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[2]));
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        fprintf(stderr, "Invalid address/ Address not supported \n");
        exit(1);
    }

    // 서버에 연결
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        exit(1);
    }

    // 소켓 옵션 출력: 연결 후
    printf("After connection:\n");
    print_socket_options(sockfd);

    close(sockfd);
    return 0;
}
