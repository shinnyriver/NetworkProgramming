#include <stdio.h>
#include <arpa/inet.h>

unsigned int convert_order32(unsigned int before){
	return htonl(before);
}

int main(void){
	unsigned int test = 0x1;
	unsigned int converted = convert_order32(test);

	if(test==converted){
		printf("System is Big Endian\n");
	}
	else{
		printf("System is Little Endian\n");
	}

	return 0;
}
