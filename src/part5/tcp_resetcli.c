#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 9000

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    struct linger linger_opt;

    // TCP 소켓 생성
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 서버 주소 설정
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr);

    // 서버에 연결
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // SO_LINGER 옵션 설정
    linger_opt.l_onoff = 1;  // LINGER 옵션 사용
    linger_opt.l_linger = 0; // LINGER 타임을 0으로 설정하여 RST 패킷 전송
    if (setsockopt(sockfd, SOL_SOCKET, SO_LINGER, &linger_opt, sizeof(linger_opt)) < 0) {
        perror("Setsockopt SO_LINGER failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 서버로 데이터 전송 (예시)
    char *message = "Hello, server!";
    send(sockfd, message, strlen(message), 0);

    // 소켓을 닫으면서 RST 패킷 전송
    close(sockfd);

    return 0;
}
