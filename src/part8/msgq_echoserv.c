#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <fcntl.h>

#define MAX_BUFSZ 512
#define RES_SEND_PROC "./res_send"

typedef struct msg {
    long msg_type;
    struct sockaddr_in addr;
    char msg_text[MAX_BUFSZ];
} msg_t;

void fork_and_exec(char *key, char *port);
void errquit(char *mesg) { perror(mesg); exit(1); }

int main(int argc, char *argv[]) {
    struct sockaddr_in servaddr;
    msg_t pmsg;
    key_t key;
    int msqid, size, nbytes, sock, port, len = sizeof(struct sockaddr);

    if(argc != 3) {
        printf("Usage: %s msgq key port\n", argv[0]);
        exit(1);
    }

    key = atoi(argv[1]);
    port = atoi(argv[2]);

    msqid = msgget(key, IPC_CREAT | 0600);
    if (msqid == -1) errquit("msgget fail");

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) errquit("socket fail");

    bzero(&servaddr, len);
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if(bind(sock, (struct sockaddr*)&servaddr, len) < 0)
        errquit("bind fail");

    fork_and_exec(argv[1], argv[2]);
    fork_and_exec(argv[1], argv[2]);
    fork_and_exec(argv[1], argv[2]);

    pmsg.msg_type = 1;
    size = sizeof(msg_t) - sizeof(long);
    puts("Server starting...");

    while(1) {
        nbytes = recvfrom(sock, pmsg.msg_text, MAX_BUFSZ, 0, 
                          (struct sockaddr *)&pmsg.addr, &len);
        if (nbytes < 0) {
            perror("recvfrom fail");
            continue;
        }

        pmsg.msg_text[nbytes] = 0;

        if (msgsnd(msqid, &pmsg, size, 0) == -1)
            errquit("msgsnd fail");
    }
    return 0;
}

void fork_and_exec(char* key, char *port) {
	pid_t pid = fork();
	if(pid<0) errquit("fork fail");
	else if(pid > 0)
		return;
	execlp(RES_SEND_PROC, RES_SEND_PROC, key, port, NULL);
	perror("execlp fail");
}
