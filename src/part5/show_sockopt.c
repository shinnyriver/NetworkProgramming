#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/types.h>

int main() {
    int sockfd;
    int optval;
    socklen_t optlen = sizeof(optval);

    // TCP 소켓 생성
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(EXIT_FAILURE);
    }

    // SO_OOBINLINE 옵션 가져오기
    if (getsockopt(sockfd, SOL_SOCKET, SO_OOBINLINE, &optval, &optlen) < 0) {
        perror("Error getting SO_OOBINLINE");
    } else {
        printf("SO_OOBINLINE: %d\n", optval);
    }

    // SO_RCVBUF 옵션 가져오기
    if (getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &optval, &optlen) < 0) {
        perror("Error getting SO_RCVBUF");
    } else {
        printf("SO_RCVBUF: %d\n", optval);
    }

    // TCP_MAXSEG 옵션 가져오기
    if (getsockopt(sockfd, IPPROTO_TCP, TCP_MAXSEG, &optval, &optlen) < 0) {
        perror("Error getting TCP_MAXSEG");
    } else {
        printf("TCP_MAXSEG: %d\n", optval);
    }

    // SO_LINGER 옵션 가져오기
    struct linger linger_opt;
    socklen_t linger_optlen = sizeof(linger_opt);
    if (getsockopt(sockfd, SOL_SOCKET, SO_LINGER, &linger_opt, &linger_optlen) < 0) {
        perror("Error getting SO_LINGER");
    } else {
        printf("SO_LINGER: %d, linger time: %d\n", linger_opt.l_onoff, linger_opt.l_linger);
    }

    // TCP_NODELAY 옵션 가져오기
    if (getsockopt(sockfd, IPPROTO_TCP, TCP_NODELAY, &optval, &optlen) < 0) {
        perror("Error getting TCP_NODELAY");
    } else {
        printf("TCP_NODELAY: %d\n", optval);
    }

    // SO_KEEPALIVE 옵션 가져오기
    if (getsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, &optval, &optlen) < 0) {
        perror("Error getting SO_KEEPALIVE");
    } else {
        printf("SO_KEEPALIVE: %d\n", optval);
    }

    // SO_SNDBUF 옵션 가져오기
    if (getsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &optval, &optlen) < 0) {
        perror("Error getting SO_SNDBUF");
    } else {
        printf("SO_SNDBUF: %d\n", optval);
    }

    // 소켓 닫기
    close(sockfd);
    return 0;
}
