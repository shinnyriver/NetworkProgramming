#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <IP> <Port> <Connections per process> <Number of processes>\n", argv[0]);
        return 1;
    }

    const char *ip = argv[1];
    const char *port = argv[2];
    const char *connections = argv[3];
    int num_processes = atoi(argv[4]);

    if (num_processes < 1) {
        fprintf(stderr, "Error: Number of processes must be at least 1.\n");
        return 1;
    }

    for (int i = 0; i < num_processes; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            continue;
        } else if (pid == 0) {
            // In child process
            execl("./busy_connect", "busy_connect", ip, port, connections, NULL);

            // If execl returns, there was an error
            perror("execl");
            exit(1);
        }
    }

    // Wait for all child processes to finish
    int status;
    while (wait(&status) > 0);

    printf("All processes completed. Check server for total connections.\n");
    return 0;
}
