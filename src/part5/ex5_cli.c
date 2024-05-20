#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAXLINE 1024

int main() {
    int sockfd;
    char buffer[MAXLINE];
    char *message = "Hello from client";
    struct sockaddr_in servaddr;

    // 소켓 생성
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // 서버 주소 설정
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    int n, len;
    len = sizeof(servaddr);

    // 서버로 메시지 전송
    sendto(sockfd, message, strlen(message), 0, (const struct sockaddr *)&servaddr, len);
    printf("Message sent\n");

    // MSG_PEEK 옵션 사용하여 메시지 확인
    n = recvfrom(sockfd, buffer, MAXLINE, MSG_PEEK, (struct sockaddr *)&servaddr, &len);
    if (n < 0) {
        perror("recvfrom");
        exit(EXIT_FAILURE);
    }
    buffer[n] = '\0';
    printf("Peeked message: %s\n", buffer);

    // 4초 대기
    sleep(4);

    // 수신 버퍼에 있는 전체 메시지 수신
    n = recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr *)&servaddr, &len);
    if (n < 0) {
        perror("recvfrom");
        exit(EXIT_FAILURE);
    }
    buffer[n] = '\0';
    printf("Received message: %s\n", buffer);

    close(sockfd);
    return 0;
}
