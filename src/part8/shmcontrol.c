// 파일명: shmcontrol.c
// 동작: 공유메모리와 세마포어를 이용한 경쟁적 접근 제어
// 컴파일: gcc -o shmcontrol shmcontrol.c -lrt
// 실행: ./shmcontrol <shmkey> <semkey>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <time.h>

void errquit(char *msg) {
    perror(msg);
    exit(1);
}

struct sembuf waitsem[] = { {0, -1, 0} };
struct sembuf notifysem[] = { {0, +1, 0} };

#define Semop(val) { if (semop(semid, val, 1) == -1) errquit("semop fail"); }

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short int *array;
    struct seminfo *__buf;
} semarg;

void fork_and_run();
void busy();
void access_shm();

char *shm_data;
int shmid, semid;

int main(int argc, char *argv[]) {
    key_t shmkey, semkey;
    unsigned short initsemval[1] = { 1 };

    if (argc < 3) {
        printf("Usage: %s <shmkey> <semkey>\n", argv[0]);
        exit(1);
    }

    shmkey = atoi(argv[1]);
    semkey = atoi(argv[2]);

    // 공유메모리 생성
    shmid = shmget(shmkey, 128, IPC_CREAT | 0660);
    if (shmid < 0)
        errquit("shmget fail");

    // 공유메모리 첨부
    shm_data = (char *)shmat(shmid, (void *)0, 0);
    if (shm_data == (char *)-1)
        errquit("shmat fail");

    // 세마포어 생성
    semid = semget(semkey, 1, IPC_CREAT | 0660);
    if (semid == -1)
        errquit("semget fail");

    // 세마포어 초기 값 설정
    semarg.array = initsemval;
    if (semctl(semid, 0, SETALL, semarg) == -1)
        errquit("semctl fail");

    fork_and_run();
    fork_and_run();
    busy();

    wait(NULL);
    wait(NULL);

    shmctl(shmid, IPC_RMID, 0); // 공유메모리 삭제
    semctl(semid, 0, IPC_RMID, 0); // 세마포어 삭제

    return 0;
}

// 자식 프로세스 생성 및 busy() 수행
void fork_and_run() {
    pid_t pid = fork();
    if (pid < 0)
        errquit("fork fail");
    else if (pid == 0) {
        busy(); // child process
        exit(0);
    }
}

void busy() {
    int i = 0;
    for (i = 0; i < 100; i++) // 100번 접근
    {
        Semop(&waitsem[0]);
        access_shm();
        Semop(&notifysem[0]);
    }
}

// 공유메모리에 접근을 하는 부분
void access_shm() {
    int i;
    pid_t pid;
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 100000000; // 0.1초

    // 공유메모리에 자신의 pid를 기록
    sprintf(shm_data, "%d", getpid());

    // 공유메모리 접근 시간
    for (i = 0; i < 1000; i++)
        pid = atoi(shm_data);

    // 공유메모리에 기록한 pid가 자신의 pid가 아니면 Error
    if (pid != getpid())
        puts("Error: 다른 프로세스도 동시에 공유메모리 접근함\n");
    else {
        putchar('.');
        fflush(stdout);
        nanosleep(&ts, NULL); // sleep
    }
}
