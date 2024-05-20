#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <netinet/tcp.h>

int main() {
	int cnt = 0;
	char line[1024];

	setvbuf(stdout, NULL, _IOLBF, 0);
	while(fgets(line, sizeof(line), stdin) != NULL)
		printf("%3i : %s", ++cnt, line);
	return 0;
}
