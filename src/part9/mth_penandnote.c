#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define PEN 0
#define NOTE 1

sem_t pen_semaphore;
sem_t note_semaphore;

void errquit(const char *mesg) {
    perror(mesg);
    exit(1);
}

void *do_work(void *arg) {
    int count = 0;
    pthread_t tid = pthread_self();

    while(count < 3) {
        sem_wait(&pen_semaphore);
        printf("[tid: %lu] 연필을 들고\n", tid);
        sem_wait(&note_semaphore);
        printf("\t[tid: %lu] 노트를 들고\n", tid);

        printf("\t[tid: %lu] 공부를 함\n", tid);

        sem_post(&pen_semaphore);
        printf("\t[tid: %lu] 연필을 내려놓음\n", tid);
        sem_post(&note_semaphore);
        printf("\t[tid: %lu] 노트를 내려놓음\n", tid);

        sleep(1);
        count++;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t threads[4];

    if (sem_init(&pen_semaphore, 0, 1) == -1) {
        errquit("sem_init (pen_semaphore)");
    }
    if (sem_init(&note_semaphore, 0, 2) == -1) {
        errquit("sem_init (note_semaphore)");
    }

    for (int i = 0; i < 4; i++) {
        if (pthread_create(&threads[i], NULL, do_work, NULL) != 0) {
            errquit("pthread_create");
        }
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&pen_semaphore);
    sem_destroy(&note_semaphore);

    return 0;
}
