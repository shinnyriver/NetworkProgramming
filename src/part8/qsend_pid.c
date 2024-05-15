#include <sys/types.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define BUFSZ 512

typedef struct msg {
    long msg_type;
    char msg_text[BUFSZ];
} msg_t;

int main(int argc, char **argv) {
    pid_t pid;
    int len, qid;
    msg_t pmsg;
    key_t key;

    if(argc != 2) {
        printf("Usage %s msqkey\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    key = atoi(argv[1]);
    if ((qid = msgget(key, IPC_CREAT | 0600)) < 0) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    if ((pid = fork()) < 0) {
        printf("fork fail");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Child process
        puts("Child process waiting to read from message queue...");
        if (msgrcv(qid, &pmsg, BUFSZ, getpid(), 0) < 0) {
            perror("msgrcv fail");
        }
        printf("Received message: %s\n", pmsg.msg_text);
        printf("Child process PID = %d\n", getpid());
        exit(EXIT_SUCCESS);
    } else {  // Parent process
        puts("Enter message to post:");
        if (fgets(pmsg.msg_text, BUFSZ, stdin) == NULL) {
            puts("No message to post");
            exit(EXIT_FAILURE);
        }
        pmsg.msg_type = getpid();  // Using parent's PID as the message type
        len = strlen(pmsg.msg_text);
        if (msgsnd(qid, &pmsg, len, 0) < 0) {
            perror("msgsnd fail");
            exit(EXIT_FAILURE);
        }
        puts("Message posted");
    }

    return 0;
}
