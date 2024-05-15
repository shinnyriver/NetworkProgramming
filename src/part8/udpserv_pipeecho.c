#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>

#define MAX_BUFSZ 512

typedef struct mesg {
    struct sockaddr_in addr;
    char data[MAX_BUFSZ];
} mesg_t;

void child_start(int sock, int pipefd[]);
void parent_start(int sock, int pipefd[]);
void errquit(char *mesg) {perror(mesg); exit(1);}

int main(int argc, char* argv[]){
    struct sockaddr_in servaddr;
    pid_t pid;
    int sock, pipefd[2], port, len = sizeof(struct sockaddr);
    if(argc != 2){
        fprintf(stderr, "Usage: %s [port number]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    port = atoi(argv[1]);
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0) {
        perror("socket error");
        exit(EXIT_FAILURE);
    }
    
    bzero(&servaddr, len);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_family=AF_INET;
    servaddr.sin_port = ntohs(port);
    bind(sock, (struct sockaddr*)&servaddr, len);

    if(pipe(pipefd) == -1) errquit("pipe error");

    pid = fork();
    if(pid < 0) errquit("fork fail");
    else if(pid > 0) parent_start(sock,pipefd);
    else child_start(sock,pipefd);
    return 0;
}

void child_start(int sock, int pipefd[]){
    mesg_t pmsg;
    int nbytes = 0, len = sizeof(struct sockaddr); close(pipefd[1]);

    while(1){
        nbytes = read(pipefd[0], (char*)&pmsg, sizeof(mesg_t));
        if(nbytes < 0) errquit("read error");
        printf("Child : %d read from pipe\n", nbytes);
        nbytes = sendto(sock, &pmsg.data, strlen(pmsg.data), 0, (struct sockaddr*)&pmsg.addr, len);
        printf("Child : %d sent to client\n", nbytes);
        printf("--------------------------------\n");
    }
}

void parent_start(int sock, int pipefd[]){
    mesg_t pmsg;
    int nbytes = 0, len = sizeof(struct sockaddr); close(pipefd[0]);

    printf("my echo server wait...\n");
    
    while(1){
        nbytes = recvfrom(sock, &pmsg.data, MAX_BUFSZ, 0, (struct sockaddr*)&pmsg.addr, &len);
        if(nbytes < 0) errquit("recvfrom error");
        printf("Parent : %d read from client\n", nbytes);
        nbytes = write(pipefd[1], (char*)&pmsg, sizeof(mesg_t));
        printf("Parent : %d write to pipe\n", nbytes);
        
    }
}