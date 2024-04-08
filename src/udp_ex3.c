#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <limits.h>

#define MAX 65535

int main(void){
	int sockets[MAX];
	int i = 0;

	for(i=0;i<MAX;i++){
		sockets[i] = socket(AF_INET, SOCK_STREAM, 0);
		if(sockets[i] < 0){
			printf("Maximum number is %d\n", i);
			break;
		}
	}

	for(int j=0; j<i;++j){
		close(sockets[j]);
	}

	return 0;
}
