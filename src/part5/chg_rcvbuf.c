#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(void){
	int s, val, len;

	if((s=socket(AF_INET, SOCK_STREAM, 0)) <0) {
		perror("socket fail");
		exit(1);
	}

	len = sizeof(val);
	if(getsockopt(s, SOL_SOCKET, SO_RCVBUF, &val, &len) < 0) {
		perror("socket fail");
		exit(1);
	}
	printf("Default Receive Buffer Size : %d\n", val);
	val = 1024;

	setsockopt(s, SOL_SOCKET, SO_RCVBUF, &val, sizeof(val));
	getsockopt(s, SOL_SOCKET, SO_RCVBUF, &val, &len);
	printf("1024 Receive Buffer Size : %d\n", val);
	return 0;
}
