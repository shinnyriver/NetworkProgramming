#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(void){
	struct servent *servent;
	servent = getservbyname("echo", "udp");
	if(servent == NULL) {
		printf("No service Information exists\n");
		exit(0);
	}

	printf("UDP ECHO Port Num(Network Order) : %d\n", servent -> s_port);
	printf("UDP ECHO Port Num(Host Order) : %d\n", ntohs(servent -> s_port));
	
	return 0;
}
