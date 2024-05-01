#include <stdio.h>
#include <arpa/inet.h>

unsigned int convert_order32(unsigned int before){
	return htonl(before);
}

int main(void){
	unsigned int before = 0x12345678;
	unsigned int after;

	after = convert_order32(before);
	printf("Before : 0x%x\n", before);
	printf("After  : 0x%x\n", after);

	return 0;
}
