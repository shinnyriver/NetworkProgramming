#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAXLINE 1024

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[MAXLINE] = {0};
    char *message = "Hello from client";

    // 소켓 생성
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
      
    // 서버 주소 변환
    if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }

    // 서버에 연결 요청
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Connection Failed");
        exit(EXIT_FAILURE);
    }

    // 메시지 전송
    send(sockfd, message, strlen(message), 0);
    printf("Message sent\n");

    // MSG_PEEK 옵션 사용하여 메시지 확인
    int n = recv(sockfd, buffer, MAXLINE, MSG_PEEK);
    if (n < 0) {
        perror("recv");
        exit(EXIT_FAILURE);
    }
    buffer[n] = '\0';
    printf("Peeked message: %s\n", buffer);

    // 4초 대기
    sleep(4);

    // 수신 버퍼에 있는 전체 메시지 수신
    n = recv(sockfd, buffer, MAXLINE, 0);
    if (n < 0) {
        perror("recv");
        exit(EXIT_FAILURE);
    }
    buffer[n] = '\0';
    printf("Received message: %s\n", buffer);

    close(sockfd);

    return 0;
}
