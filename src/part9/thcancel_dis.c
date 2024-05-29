#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <limits.h>
#include <errno.h>

void *thrfunc(void *arg);

void cancel_and_join(int tid);
int max_loop=20000000;
int cnt = 0;
pthread_t curthd;

int main(int argc, char **argv){
    pthread_t tid;
    int status, i;
    struct timespec micro_sec = {0,100000};

    printf("** PTHREAD_CANCEL_DISABLE\n");
    if((status=pthread_create(&tid, NULL, &thrfunc, NULL))!=0) {
        printf("thread create error: %s\n", strerror(status));
        exit(0);
    }

    nanosleep(&micro_sec, NULL);
    cancel_and_join(tid);
    return 0;
}

void *thrfunc(void *arg) {
    int status;
    curthd = pthread_self();
    if((status=pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL))!=0) {
        printf("pthread_setcancelstate error: %s\n", strerror(status));
        exit(0);
    }

    for(cnt = 1; cnt<max_loop; cnt++) {
        if(cnt%500000 == 0)
            pthread_testcancel();
    }
}

void cancel_and_join(int tid) {
    void * result;
    int status;
    if((status=pthread_cancel(tid))!=0) {
        printf("pthread_cancel error: %s\n", strerror(status));
        exit(0);
    }

    if((status=pthread_join(tid, &result))!=0) {
        printf("pthread_join error: %s\n", strerror(status));
        exit(0);
    }

    if(result == PTHREAD_CANCELED)
        printf("[Thread ID=%ld] thread is canceled\n", curthd);
    else
        printf("[Thread ID=%ld] thread is not canceled\n", curthd);

    printf("%d loop / %d loop", max_loop, cnt);
    return ;
}
