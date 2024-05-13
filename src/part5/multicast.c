//--------------------------------------------------------------
// Filename: multicast.c
// Purpose: Multicast chat program
// Compilation: cc -o multicast multicast.c
// Usage: multicast 239.0.3.3 3000 MyName
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define MAXLINE 1023

int main(int argc, char *argv[]) {
    int sends, recvs, n, len;
    pid_t pid;
    unsigned int yes = 1;
    struct sockaddr_in mcast_group;
    struct ip_mreq mreq;

    char name[10];
    if (argc != 4) {
        printf("Usage: %s multicast_address port MyName\n", argv[0]);
        exit(0);
    }

    sprintf(name, "[%s]", argv[3]);

    // Setup multicast receive socket
    memset(&mcast_group, 0, sizeof(mcast_group));
    mcast_group.sin_family = AF_INET;
    mcast_group.sin_port = htons(atoi(argv[2]));
    inet_pton(AF_INET, argv[1], &mcast_group.sin_addr);

    if ((recvs = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Error: Can't create receive socket\n");
        exit(0);
    }

    // Join multicast group
    mreq.imr_multiaddr = mcast_group.sin_addr;
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if (setsockopt(recvs, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
        printf("Error: add membership\n");
        exit(0);
    }

    // Set socket reuse option
    if (setsockopt(recvs, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
        printf("Error: reuse setsockopt\n");
        exit(0);
    }

    // Bind receive socket
    if (bind(recvs, (struct sockaddr*)&mcast_group, sizeof(mcast_group)) < 0) {
        printf("Error: bind receive socket\n");
        exit(0);
    }

    // Setup multicast send socket
    if ((sends = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("Error: Can't create send socket\n");
        exit(0);
    }

    // Fork process: child handles receiving, parent handles sending
    if ((pid = fork()) < 0) {
        printf("Error: fork\n");
        exit(0);
    }

    if (pid == 0) { // Child process - Receiving chat messages
        struct sockaddr_in from;
        char message[MAXLINE + 1];

        for (;;) {
            printf("Receiving message...\n");
            len = sizeof(from);
            if ((n = recvfrom(recvs, message, MAXLINE, 0, (struct sockaddr*)&from, &len)) < 0) {
                printf("Error: recvfrom\n");
                exit(0);
            }
            message[n] = 0;
            printf("Received Message: %s\n", message);
        }
    } else { // Parent process - Sending chat messages
        char message[MAXLINE + 1], line[MAXLINE + 1];

        printf("Send Message:");
        while (fgets(message, MAXLINE, stdin) != NULL) {
            sprintf(line, "%s %s", name, message);
            len = strlen(line);
            if (sendto(sends, line, len, 0, (struct sockaddr*)&mcast_group, sizeof(mcast_group)) < len) {
                printf("Error: sendto\n");
                exit(0);
            }
        }
    }

    return 0;
}
