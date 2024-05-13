#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 9000

int main() {
    int listenfd, connfd;
    struct sockaddr_in servaddr;
    char buffer[1024];
    ssize_t n;

    // 소켓 생성 및 바인드
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    listen(listenfd, 5);

    // 클라이언트 연결 대기
    connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);

    // 데이터 수신
    n = recv(connfd, buffer, sizeof(buffer), 0);
    if (n <= 0) {
        if (n == 0) {
            printf("The client closed the connection\n");
        } else {
            perror("recv error");
        }
    } else {
        buffer[n] = '\0';
        printf("Received: %s\n", buffer);
    }

    // 소켓 닫기
    close(connfd);
    close(listenfd);

    return 0;
}
