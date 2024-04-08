#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define MSG "Test Message"

int main() {
	int sd1, sd2;
	int pr1, pr2;
	int addrlen;
	struct sockaddr_in servaddr, cliaddr;
	unsigned short port1, port2, port3;

	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(7);

	sd1=socket(PF_INET, SOCK_STREAM, 0);
	sd2=socket(PF_INET, SOCK_DGRAM, 0);

	if(connect(sd1, (struct sockaddr*)&servaddr, sizeof(servaddr))<0){
		perror("connect fail");
		exit(1);
	}
	getsockname(sd1, (struct sockaddr*)&cliaddr, &addrlen);
	getpeername(pr1, (struct sockaddr*)&servaddr, &addrlen);
	port1 = ntohs(cliaddr.sin_port);
	port3 = ntohs(servaddr.sin_port);

	sendto(sd2, MSG, strlen(MSG), 0, (struct sockaddr*)&servaddr, sizeof(servaddr));
	getsockname(sd2, (struct sockaddr*)&cliaddr, &addrlen);
	getpeername(pr2, (struct sockaddr*)&servaddr, &addrlen);
	port2 = ntohs(cliaddr.sin_port);

	printf("Stream Socket Port Number : %d\n", port1);
	printf("Datagram Socket Port Number : %d\n", port2);
	printf("peer socket port number : %d\n", port3);
	close(sd1);
	close(sd2);
	close(pr1);
	close(pr2);

	return 0;
}

