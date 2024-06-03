#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

void *thrfunc(void *arg);
int counting = 0;
pthread_mutex_t count_lock;
pthread_mutexattr_t mutex_attr;

int main(int argc, char **argv) {
	pthread_t tid[2];
	int i, status;
	pthread_mutexattr_init(&mutex_attr);
	for(i=0; i<2; i++){
		if((status=pthread_create(&tid[i], NULL, &thrfunc, NULL))!=0) {
		printf("pthread_create fail: %s", strerror(status));
	exit(0);
		}

	}

	for(i=0; i<2; i++) pthread_join(tid[0], NULL);
	return 0;
}

void *thrfunc(void *arg) {
	while(1) {
		pthread_mutex_lock(&count_lock);
		printf("\n[%ld Thread] Mutex Lock\n", pthread_self());
		printf("[%ld Thread] Counting = %d\n", pthread_self(), counting);
		counting++;
		sleep(1);
		printf("[%ld Thread] Mutex Unlock\n", pthread_self());
		pthread_mutex_unlock(&count_lock);
	}
	return NULL;
}
