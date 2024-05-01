#include <stdio.h>
#include <stdlib.h>

int main(void) {
	const char* value = getenv("myenv");

	if(value==NULL){
		printf("error occured!\n");
	}

	else{
		extern char **environ;
		for(int i=0; environ[i]!=NULL;i++)
			printf("%s\n",environ[i]);
	}

	return 0;
}
