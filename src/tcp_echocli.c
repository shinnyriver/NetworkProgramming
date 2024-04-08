#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define MAXLINE 127

int main(int argc, char* argv[]){
	struct sockaddr_in servaddr, cliaddr;
	int s, nbyte;
	char buf[MAXLINE+1];
        short int port1, port2;
	int addrlen;

	if(argc != 2) {
		printf("usage: %s ip_address\n", argv[0]);
		exit(0);
	}

	if((s=socket(PF_INET, SOCK_STREAM, 0)) <0 ){
		perror("socket fail");
		exit(0);
	}

	bzero((char*)&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
	servaddr.sin_port = htons(7);

	if(connect(s, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
		perror("connect fail");
		exit(0);
	}

	printf("Insert : ");
	if(fgets(buf, sizeof(buf), stdin) == NULL) exit(0);
	nbyte = strlen(buf);

	if(write(s, buf, nbyte) < 0) {
		printf("write error\n");
		exit(0);
	}
	printf("Receive : ");
	if((nbyte=read(s,buf,MAXLINE)) < 0) {
		perror("read fail");
		exit(0);
	}
	
	int sd1, sd2;
	sd1=socket(PF_INET, SOCK_STREAM, 0);
	sd2=socket(PF_INET, SOCK_STREAM, 0);
	getsockname(sd1, (struct sockaddr*)&cliaddr, &addrlen);
	getpeername(sd2, (struct sockaddr*)&servaddr, &addrlen);
	printf("stream socket port num : %d\n", sd1);
	printf("peer socket port num : %d\n", sd2);
	buf[nbyte]=0;
	printf("%s", buf);
	close(s);
	return 0;
}
