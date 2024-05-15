#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int msq_remove(int qid);
int view_qinfo(int qid);

int main(int argc, char **argv) {
    int qid;
    key_t key;

    if (argc != 2) {
        printf("USAGE: %s msqkey\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    key = atoi(argv[1]);

    // 메시지큐 ID 얻기
    if ((qid = msgget(key, 0)) < 0) {
        perror("msgget fail");
        exit(0);
    }

    view_qinfo(qid);
    msq_remove(qid);

    exit(EXIT_SUCCESS);
}

// 메시지큐 정보 출력 함수
int view_qinfo(int qid) {
    struct msqid_ds buf;
    struct ipc_perm *pm;

    if ((msgctl(qid, IPC_STAT, &buf)) < 0) {
        perror("msgctl");
        return -1;
    }

    pm = &buf.msg_perm;
    printf("Queue's max byte size: %d\n", buf.msg_qbytes);
    printf("Effective user UID: %d\n", pm->uid);
    printf("Effective user GID: %d\n", pm->gid);
    printf("Queue access permissions: %o\n", pm->mode);

    return 0;
}

// 메시지큐 제거 함수
int msq_remove(int qid) {
    if ((msgctl(qid, IPC_RMID, NULL)) < 0) {
        perror("msgctl");
        return -1;
    }

    printf("Message queue %d deleted\n", qid);
    return 0;
}
