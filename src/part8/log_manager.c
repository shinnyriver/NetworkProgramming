#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define LOG_MSG_TYPE 1
#define MAX_TEXT 512

typedef struct {
    long msg_type;
    char msg_text[MAX_TEXT];
} log_msg_t;

void log_to_file(const char *logfile, const char *message) {
    FILE *file = fopen(logfile, "a");
    if (file == NULL) {
        perror("fopen");
        exit(1);
    }
    fprintf(file, "%s\n", message);
    fclose(file);
}

int main() {
    key_t key = ftok("logfile", 65);
    int msgid = msgget(key, 0666 | IPC_CREAT);
    log_msg_t log_msg;
    const char *logfile = "server.log";

    while (1) {
        msgrcv(msgid, &log_msg, sizeof(log_msg.msg_text), LOG_MSG_TYPE, 0);
        log_to_file(logfile, log_msg.msg_text);
    }

    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}
