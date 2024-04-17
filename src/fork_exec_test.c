#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void child_start();

int main(int argc, char **argv){
	pid_t pid;
	int chile_status, child_return;

	puts("\t parent process start.......");
	if((pid=fork()) < 0) {
		perror("fork failed : ");
		exit(0);
	}

	else if(pid == 0) child_start();
	else if(pid>0)
		printf("\n\r** parent : [my pid: %d] my child pid = %d\n", getpid(), pid);
	return 0;
}

void child_start() {
	puts("\t child process start...");
	printf("\t** child : [my pid: %d] my parent pid = %d\n",
			getpid(), getppid());
	printf("\n\t** exec() function ls command\n");
	execlp("ls", "ls", NULL);

	perror("exec error at child: ");
	exit(0);
}
